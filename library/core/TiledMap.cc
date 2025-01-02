// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/TiledMap.h>

#include <algorithm>
#include <array>
#include <charconv>
#include <fstream>
#include <optional>

#include <fmt/std.h>
#include <pugixml.hpp>
#include <zlib.h>

#include <gf2/core/Blit.h>
#include <gf2/core/Log.h>
#include <gf2/core/Property.h>
#include <gf2/core/ResourceBundle.h>
#include <gf2/core/StringUtils.h>

using namespace std::literals;

namespace gf {

  namespace {

    /*
     * support
     */

    void unsupported_attribute(const pugi::xml_node node, const char* attribute_name)
    {
      auto attribute = node.attribute(attribute_name);

      if (!attribute.empty()) {
        Log::warning("Unsupported attribute '{}' in node '{}'.", attribute_name, node.name());
      }
    }

    void unsupported_node(const pugi::xml_node node, const char* child_name)
    {
      auto child = node.child(child_name);

      if (!child.empty()) {
        Log::warning("Unsupported node '{}' in node '{}'.", child_name, node.name());
      }
    }

    uint8_t convert_hex_char(char c)
    {
      if ('0' <= c && c <= '9') {
        return c - '0';
      }

      if ('a' <= c && c <= 'f') {
        return c - 'a' + 10;
      }

      if ('A' <= c && c <= 'F') {
        return c - 'A' + 10;
      }

      Log::error("Invalid character: '{}' ({#X}).", c, c);
      return 0;
    }

    Color parse_color(pugi::xml_attribute attribute, Color default_value = White)
    {
      if (attribute.empty()) {
        return default_value;
      }

      std::string value = attribute.as_string();
      assert(!value.empty());

      if (value[0] == '#') {
        value.erase(value.begin());
      }

      switch (value.size()) {
        case 5:
          value.insert(0, 1, '0');
          [[fallthrough]];
        case 6:
          {
            uint32_t rgb = 0;

            for (std::size_t i = 0; i < 6; ++i) {
              rgb = (rgb << 4) + convert_hex_char(value[i]);
            }

            return { rgb };
          }

        case 7:
          value.insert(0, 1, '0');
          [[fallthrough]];
        case 8:
          {
            const uint8_t alpha = (convert_hex_char(value[0]) << 4) + convert_hex_char(value[1]);
            uint32_t rgb = 0;

            for (std::size_t i = 2; i < 8; ++i) {
              rgb = (rgb << 4) + convert_hex_char(value[i]);
            }

            return { rgb, alpha };
          }

        default:
          Log::error("Unknown color format: '{}'.", value);
          break;
      }

      return default_value;
    }

    template<typename T>
    T parse_number(std::string_view string)
    {
      while (string.front() == '\n') {
        string.remove_prefix(1);
      }

      T value = {};
      [[maybe_unused]] auto [ptr, ec] = std::from_chars(string.data(), string.data() + string.size(), value);
      assert(ec == std::errc());
      return value;
    }

    /*
     * properties
     */

    // NOLINTNEXTLINE(misc-no-recursion)
    PropertyMap parse_tmx_raw_properties(const pugi::xml_node node)
    {
      PropertyMap property_map;

      for (const pugi::xml_node properties : node.children("properties")) {
        for (const pugi::xml_node property : properties.children("property")) {
          std::string name = property.attribute("name").as_string();
          assert(!name.empty());

          auto type_attribute = property.attribute("type");
          auto value_attribute = property.attribute("value");
          assert(value_attribute);

          if (!type_attribute.empty()) {
            std::string type = type_attribute.as_string();

            if (type == "string") {
              property_map.add_property(std::move(name), std::string(value_attribute.as_string()));
            } else if (type == "int") {
              property_map.add_property(std::move(name), static_cast<int64_t>(value_attribute.as_int()));
            } else if (type == "float") {
              property_map.add_property(std::move(name), value_attribute.as_double());
            } else if (type == "bool") {
              property_map.add_property(std::move(name), value_attribute.as_bool());
            } else if (type == "color") {
              property_map.add_property(std::move(name), parse_color(value_attribute));
            } else if (type == "file") {
              property_map.add_property(std::move(name), std::filesystem::path(value_attribute.as_string()));
            } else if (type == "object") {
              property_map.add_property(std::move(name), gf::hash_string(value_attribute.as_string()));
            } else if (type == "class") {
              property_map.add_property(std::move(name), parse_tmx_raw_properties(property));
            } else {
              Log::error("Wrong type string: '{}'.", type);
            }

          } else {
            property_map.add_property(std::move(name), std::string(value_attribute.as_string()));
          }
        }
      }

      return property_map;
    }

    uint32_t parse_tmx_properties(const pugi::xml_node node, TiledMap& map)
    {
      PropertyMap property_map = parse_tmx_raw_properties(node);

      if (property_map.empty()) {
        return NoIndex;
      }

      auto index = static_cast<uint32_t>(map.properties.size());
      map.properties.push_back(std::move(property_map));
      return index;
    }

    /*
     * layers
     */

    MapLayer parse_tmx_layer_common(const pugi::xml_node node, TiledMap& map)
    {
      MapLayer layer;
      layer.properties_index = parse_tmx_properties(node, map);
      layer.name = node.attribute("name").as_string();
      layer.offset.x = node.attribute("offsetx").as_int(0);
      layer.offset.y = node.attribute("offsety").as_int(0);
      unsupported_attribute(node, "opacity");
      unsupported_attribute(node, "visible");
      return layer;
    }

    // tile layer

    enum class TmxFormat : uint8_t {
      Xml,
      Base64,
      Base64_Zlib,
      Base64_Gzip,
      Csv,
    };

    std::optional<uint8_t> parse_data_base64_byte(char c)
    {
      if (c >= 'A' && c <= 'Z') {
        return c - 'A';
      }

      if (c >= 'a' && c <= 'z') {
        return c - 'a' + 26;
      }

      if (c >= '0' && c <= '9') {
        return c - '0' + 52;
      }

      if (c == '+') {
        return 0x3E;
      }

      if (c == '/') {
        return 0x3F;
      }

      return std::nullopt;
    }

    std::size_t parse_data_base64_compute_padding(const std::string& input)
    {
      const std::size_t len = input.size();
      size_t padding = 0;

      if (len >= 2) {
        if (input[len - 1] == '=') {
          ++padding;
        }

        if (input[len - 2] == '=') {
          ++padding;
        }
      }

      return padding;
    }

    // http://en.wikibooks.org/wiki/Algorithm_implementation/Miscellaneous/Base64
    std::vector<uint8_t> parse_data_base64(std::string input)
    {
      input.erase(std::remove_if(input.begin(), input.end(), [](char c) { return c == '\n' || c == ' '; }), input.end());
      const std::size_t len = input.size();
      assert(len % 4 == 0);
      const size_t padding = parse_data_base64_compute_padding(input);

      // Setup a vector to hold the result
      std::vector<uint8_t> decoded;
      decoded.reserve(((len / 4) * 3) - padding);

      for (std::size_t i = 0; i < len;) {

        uint32_t tmp = 0; // Holds decoded quanta
        for (int k = 0; k < 4; ++k) {
          tmp <<= 6;
          char c = input[i];

          if (auto maybe_byte = parse_data_base64_byte(c); maybe_byte) {
            tmp |= *maybe_byte;
          } else if (c == '=') { // pad
            switch (len - i) {
              case 1: // One pad character
                decoded.push_back((tmp >> 16) & 0x000000FF);
                decoded.push_back((tmp >> 8) & 0x000000FF);
                return decoded;
              case 2: // Two pad characters
                decoded.push_back((tmp >> 10) & 0x000000FF);
                return decoded;
              default:
                assert(false);
            }
          } else {
            Log::fatal("Unknown character: '{}' ({#X}).", c, c);
          }

          ++i;
        }

        // clang-format off
        decoded.push_back((tmp >> 16) & 0x000000FF);
        decoded.push_back((tmp >> 8 ) & 0x000000FF);
        decoded.push_back((tmp      ) & 0x000000FF);
        // clang-format on
      }

      return decoded;
    }

    std::vector<uint8_t> parse_data_compressed(std::vector<uint8_t> input)
    {
      std::vector<uint8_t> uncompressed;
      std::array<uint8_t, 1024> tmp = {};

      z_stream stream = {};
      stream.next_in = input.data();
      stream.avail_in = static_cast<uInt>(input.size());
      stream.zalloc = Z_NULL;
      stream.zfree = Z_NULL;

      inflateInit2(&stream, 15 + 32); // allow to decode gzip and zlib format

      for (;;) {
        stream.next_out = tmp.data();
        stream.avail_out = static_cast<uInt>(tmp.size());

        const int err = inflate(&stream, Z_SYNC_FLUSH);

        if (err == Z_OK) {
          uncompressed.insert(uncompressed.end(), tmp.begin(), tmp.end() - stream.avail_out);
        } else if (err == Z_STREAM_END) {
          uncompressed.insert(uncompressed.end(), tmp.begin(), tmp.end() - stream.avail_out);
          break;
        } else {
          assert(false);
        }
      }

      assert(stream.avail_in == 0);
      inflateEnd(&stream);
      return uncompressed;
    }

    TmxFormat parse_data_format(const pugi::xml_node node)
    {
      assert(node.name() == "data"sv);

      const std::string encoding = node.attribute("encoding").as_string();

      if (encoding == "csv") {
        return TmxFormat::Csv;
      }

      if (encoding == "base64") {
        const std::string compression = node.attribute("compression").as_string();

        if (compression == "zlib") {
          return TmxFormat::Base64_Zlib;
        }

        if (compression == "gzip") {
          return TmxFormat::Base64_Gzip;
        }

        return TmxFormat::Base64;
      }

      return TmxFormat::Xml;
    }

    std::vector<uint8_t> parse_data_buffer(const pugi::xml_node node, TmxFormat format)
    {
      assert(node.name() == "data"sv || node.name() == "chunk"sv);

      std::vector<uint8_t> data;

      switch (format) {
        case TmxFormat::Xml:
        case TmxFormat::Csv:
          assert(false);
          break;

        case TmxFormat::Base64:
          data = parse_data_base64(node.child_value());
          break;

        case TmxFormat::Base64_Zlib:
        case TmxFormat::Base64_Gzip:
          data = parse_data_compressed(parse_data_base64(node.child_value()));
          break;
      }

      return data;
    }

    MapTile parse_gid(uint32_t gid)
    {
      static constexpr uint32_t FlippedHorizontallyBit = UINT32_C(0x80000000);
      static constexpr uint32_t FlippedVerticallyBit = UINT32_C(0x40000000);
      static constexpr uint32_t FlippedDiagonallyBit = UINT32_C(0x20000000);
      static constexpr uint32_t RotatedHexagonal120Bit = UINT32_C(0x10000000);

      MapTile tile;

      if ((gid & FlippedHorizontallyBit) != 0) {
        tile.flip.set(CellFlip::Horizontally);
      }

      if ((gid & FlippedVerticallyBit) != 0) {
        tile.flip.set(CellFlip::Vertically);
      }

      if ((gid & FlippedDiagonallyBit) != 0) {
        tile.flip.set(CellFlip::Diagonally);
      }

      if ((gid & RotatedHexagonal120Bit) != 0) {
        tile.flip.set(CellFlip::Rotation120);
      }

      tile.gid = gid & ~(FlippedHorizontallyBit | FlippedVerticallyBit | FlippedDiagonallyBit | RotatedHexagonal120Bit);
      return tile;
    }

    std::vector<MapTile> parse_tmx_tiles(const pugi::xml_node node, TmxFormat format)
    {
      std::vector<MapTile> tiles;

      switch (format) {
        case TmxFormat::Base64:
        case TmxFormat::Base64_Zlib:
        case TmxFormat::Base64_Gzip:
          {
            std::vector<uint8_t> buffer = parse_data_buffer(node, format);

            const std::size_t size = buffer.size();
            assert(size % 4 == 0);

            for (std::size_t i = 0; i < size; i += 4) {
              const uint32_t gid = buffer[i] | (buffer[i + 1] << 8) | (buffer[i + 2] << 16) | (buffer[i + 3] << 24);
              tiles.push_back(parse_gid(gid));
            }

            break;
          }

        case TmxFormat::Csv:
          {
            auto items = split_string(node.child_value(), ",");

            for (auto item : items) {
              tiles.push_back(parse_gid(parse_number<uint32_t>(item)));
            }

            break;
          }

        case TmxFormat::Xml:
          for (const pugi::xml_node tile : node.children("tile")) {
            tiles.push_back(parse_gid(tile.attribute("gid").as_uint()));
          };

          break;
      }

      return tiles;
    }

    MapLayerStructure parse_tmx_tile_layer(const pugi::xml_node node, TiledMap& map)
    {
      assert(node.name() == "layer"sv);
      assert(map.map_size.w == node.attribute("width").as_int());
      assert(map.map_size.h == node.attribute("height").as_int());

      MapTileLayer tile_layer;
      tile_layer.layer = parse_tmx_layer_common(node, map);

      for (const pugi::xml_node data_node : node.children("data")) {
        tile_layer.tiles = Array2D<MapTile>(map.map_size);

        auto format = parse_data_format(data_node);
        auto range = data_node.children("chunk");

        if (std::distance(range.begin(), range.end()) > 0) {
          for (const pugi::xml_node chunk_node : range) {
            Vec2I chunk_position = {};
            chunk_position.x = chunk_node.attribute("x").as_int();
            chunk_position.y = chunk_node.attribute("y").as_int();

            Vec2I chunk_size = {};
            chunk_size.w = chunk_node.attribute("width").as_int();
            chunk_size.h = chunk_node.attribute("height").as_int();

            auto blit = compute_blit(RectI::from_size(chunk_size), chunk_size, chunk_position, tile_layer.tiles.size());
            auto tiles = parse_tmx_tiles(chunk_node, format);

            for (auto offset : gf::position_range(blit.source_region.size())) {
              const std::size_t index = (offset.y * chunk_size.w) + offset.x;
              const Vec2I position = blit.target_offset + offset;
              tile_layer.tiles(position) = tiles[index];
            }
          }
        } else {
          auto tiles = parse_tmx_tiles(data_node, format);
          assert(tiles.size() == static_cast<std::size_t>(map.map_size.w * map.map_size.h));
          std::copy(tiles.begin(), tiles.end(), tile_layer.tiles.begin());
        }
      }

      MapLayerStructure structure = {};
      structure.type = MapLayerType::Tile;
      structure.layer_index = static_cast<uint32_t>(map.tile_layers.size());

      map.tile_layers.push_back(std::move(tile_layer));

      return structure;
    }

    // object layer

    std::vector<Vec2F> parse_points(std::string_view points_string)
    {
      std::vector<Vec2F> points;

      auto items = split_string(points_string, " ");

      for (auto item : items) {
        auto coordinates = split_string(item, ",");
        assert(coordinates.size() == 2);
        points.emplace_back(parse_number<float>(coordinates[0]), parse_number<float>(coordinates[1]));
      }

      return points;
    }

    MapObject parse_tmx_object(const pugi::xml_node node, TiledMap& map)
    {
      MapObject object;
      object.properties_index = parse_tmx_properties(node, map);
      object.id = gf::id(node.attribute("id").as_int());
      object.name = node.attribute("name").as_string();
      object.location.x = node.attribute("x").as_float();
      object.location.y = node.attribute("y").as_float();
      object.rotation = node.attribute("rotation").as_float();

      unsupported_attribute(node, "type"); // TODO?
      unsupported_attribute(node, "visible");

      if (const pugi::xml_node polygon = node.child("polygon"); polygon) {
        object.type = MapObjectType::Polygon;
        object.feature = parse_points(polygon.attribute("points").as_string());
      } else if (const pugi::xml_node polyline = node.child("polyline"); polyline) {
        object.type = MapObjectType::Polyline;
        object.feature = parse_points(polyline.attribute("points").as_string());
      } else if (!node.child("text").empty()) {
        unsupported_node(node, "text");
      } else if (const pugi::xml_attribute gid = node.attribute("gid"); gid) {
        object.type = MapObjectType::Tile;
        object.feature = parse_gid(gid.as_uint());
      } else if (!node.child("point").empty()) {
        object.type = MapObjectType::Point;
      } else if (!node.child("ellipse").empty()) {
        object.type = MapObjectType::Ellipse;
        const Vec2F size = { node.attribute("width").as_float(), node.attribute("height").as_float() };
        object.feature = size;
      } else {
        object.type = MapObjectType::Rectangle;
        const Vec2F size = { node.attribute("width").as_float(), node.attribute("height").as_float() };
        object.feature = size;
      }

      return object;
    }

    MapLayerStructure parse_tmx_object_layer(const pugi::xml_node node, TiledMap& map)
    {
      assert(node.name() == "objectgroup"sv);

      MapObjectLayer object_layer;
      object_layer.layer = parse_tmx_layer_common(node, map);

      unsupported_attribute(node, "color");
      unsupported_attribute(node, "draworder");

      for (const pugi::xml_node object : node.children("object")) {
        object_layer.objects.push_back(parse_tmx_object(object, map));
      }

      MapLayerStructure structure = {};
      structure.type = MapLayerType::Object;
      structure.layer_index = static_cast<uint32_t>(map.object_layers.size());

      map.object_layers.push_back(std::move(object_layer));

      return structure;
    }

    // group layer

    std::vector<MapLayerStructure> parse_tmx_layer_structure(pugi::xml_node node, TiledMap& map);

    // NOLINTNEXTLINE(misc-no-recursion)
    MapLayerStructure parse_tmx_group_layer(const pugi::xml_node node, TiledMap& map)
    {
      assert(node.name() == "group"sv);

      MapGroupLayer group_layer = {};
      group_layer.layer = parse_tmx_layer_common(node, map);
      group_layer.sub_layers = parse_tmx_layer_structure(node, map);

      MapLayerStructure structure = {};
      structure.type = MapLayerType::Group;
      structure.layer_index = static_cast<uint32_t>(map.group_layers.size());

      map.group_layers.push_back(std::move(group_layer));

      return structure;
    }

    // NOLINTNEXTLINE(misc-no-recursion)
    std::vector<MapLayerStructure> parse_tmx_layer_structure(const pugi::xml_node node, TiledMap& map)
    {
      std::vector<MapLayerStructure> layers;

      for (const pugi::xml_node layer : node.children()) {
        const std::string name = layer.name();

        if (name == "layer") {
          layers.push_back(parse_tmx_tile_layer(layer, map));
        } else if (name == "objectgroup") {
          layers.push_back(parse_tmx_object_layer(layer, map));
        } else if (name == "group") {
          layers.push_back(parse_tmx_group_layer(layer, map));
        }
      }

      return layers;
    }

    /*
     * tileset
     */

    struct MapTilesetTileComparator {
      constexpr bool operator()(const MapTilesetTile& lhs, const MapTilesetTile& rhs)
      {
        return lhs.id < rhs.id;
      }

      constexpr bool operator()(uint32_t lhs, const MapTilesetTile& rhs)
      {
        return lhs < rhs.id;
      }

      constexpr bool operator()(const MapTilesetTile& lhs, uint32_t rhs)
      {
        return lhs.id < rhs;
      }
    };

    MapTilesetTile parse_tmx_tileset_tile(const pugi::xml_node node, TiledMap& map)
    {
      assert(node.name() == "tile"sv);
      MapTilesetTile tile;
      tile.properties_index = parse_tmx_properties(node, map);
      tile.id = node.attribute("id").as_int();
      tile.type = node.attribute("type").as_string();

      const pugi::xml_node objects = node.child("objectgroup");

      if (!objects.empty()) {
        tile.objects = parse_tmx_object_layer(objects, map).layer_index;
      }

      unsupported_node(node, "image");
      unsupported_node(node, "animation");

      return tile;
    }

    void parse_tmx_tileset_from_element(MapTileset& tileset, const pugi::xml_node node, TiledMap& map, const std::filesystem::path& base_directory)
    {
      assert(node.name() == "tileset"sv);

      tileset.tile_size.w = node.attribute("tilewidth").as_int();
      tileset.tile_size.h = node.attribute("tileheight").as_int();

      if (const pugi::xml_node offset = node.child("tileoffset"); offset != nullptr) {
        tileset.offset.x = offset.attribute("x").as_int();
        tileset.offset.y = offset.attribute("y").as_int();
      }

      tileset.spacing = node.attribute("spacing").as_int();
      tileset.margin = node.attribute("margin").as_int();

      const pugi::xml_node image = node.child("image");
      assert(image);
      std::filesystem::path image_path = base_directory / image.attribute("source").as_string();
      tileset.texture_index = static_cast<uint32_t>(map.textures.size());
      map.textures.push_back(std::move(image_path));

      for (const pugi::xml_node tile : node.children("tile")) {
        tileset.tiles.push_back(parse_tmx_tileset_tile(tile, map));
      }

      std::sort(tileset.tiles.begin(), tileset.tiles.end(), MapTilesetTileComparator());

      unsupported_node(node, "wangsets");
    }

    void parse_tmx_tileset_from_file(MapTileset& tileset, const std::filesystem::path& source, TiledMap& map, const std::filesystem::path& base_directory)
    {
      std::filesystem::path tileset_path = base_directory / source;
      std::ifstream tileset_file(tileset_path);

      if (!tileset_file) {
        Log::fatal("Unknown TSX file: '{}'.", tileset_path);
      }

      pugi::xml_document doc;
      const pugi::xml_parse_result result = doc.load(tileset_file);

      if (!result) {
        Log::fatal("Could not load TSX file '{}': {}.", tileset_path, result.description());
      }

      const pugi::xml_node node = doc.child("tileset");

      if (!node.attribute("firstgid").empty()) {
        Log::warning("Attribute 'firstgid' present in a TSX file: '{}'.", tileset_path);
      }

      if (!node.attribute("source").empty()) {
        Log::warning("Attribute 'source' present in a TSX file: '{}'.", tileset_path);
      }

      parse_tmx_tileset_from_element(tileset, node, map, tileset_path.parent_path());
    }

    MapTileset parse_tmx_tileset(const pugi::xml_node node, TiledMap& map, const std::filesystem::path& base_directory)
    {
      assert(node.name() == "tileset"sv);
      MapTileset tileset;
      tileset.properties_index = parse_tmx_properties(node, map);
      tileset.first_gid = node.attribute("firstgid").as_uint();

      const std::filesystem::path source = node.attribute("source").as_string();

      if (!source.empty()) {
        parse_tmx_tileset_from_file(tileset, source, map, base_directory);
      } else {
        parse_tmx_tileset_from_element(tileset, node, map, base_directory);
      }

      return tileset;
    }

    /*
     * map
     */

    void parse_tmx_map(const pugi::xml_node node, TiledMap& map, const std::filesystem::path& base_directory)
    {
      assert(node.name() == "map"sv);

      map.properties_index = parse_tmx_properties(node, map);

      std::string orientation = node.attribute("orientation").as_string();
      assert(!orientation.empty());

      if (orientation == "orthogonal") {
        map.orientation = GridOrientation::Orthogonal;
      } else if (orientation == "isometric") {
        map.orientation = GridOrientation::Isometric;
      } else if (orientation == "staggered") {
        map.orientation = GridOrientation::Staggered;
      } else if (orientation == "hexagonal") {
        map.orientation = GridOrientation::Hexagonal;
      } else {
        Log::error("Wrong orientation string: '{}'.", orientation);
      }

      map.map_size.w = node.attribute("width").as_int();
      map.map_size.h = node.attribute("height").as_int();

      map.tile_size.w = node.attribute("tilewidth").as_int();
      map.tile_size.h = node.attribute("tileheight").as_int();

      map.hex_side_length = node.attribute("hexsidelength").as_int(0);

      std::string cell_axis = node.attribute("staggeraxis").as_string("x");

      if (cell_axis == "x") {
        map.cell_axis = CellAxis::X;
      } else if (cell_axis == "y") {
        map.cell_axis = CellAxis::Y;
      } else {
        Log::error("Wrong cell axis string: '{}'.", cell_axis);
      }

      std::string cell_index = node.attribute("staggerindex").as_string("odd");

      if (cell_index == "odd") {
        map.cell_index = CellIndex::Odd;
      } else if (cell_index == "even") {
        map.cell_index = CellIndex::Even;
      } else {
        Log::error("Wrong cell index string: '{}'.", cell_index);
      }

      unsupported_attribute(node, "backgroundcolor");

      for (const pugi::xml_node tileset : node.children("tileset")) {
        map.tilesets.push_back(parse_tmx_tileset(tileset, map, base_directory));
      }

      map.layers = parse_tmx_layer_structure(node, map);
    }

  }

  /*
   * MapTileset
   */

  Vec2I MapTileset::compute_layout(Vec2I texture_size) const
  {
    if (tile_size.w == 0 || tile_size.h == 0) {
      return { 0, 0 };
    }

    return (texture_size - 2 * margin + spacing) / (tile_size + spacing);
  }

  RectF MapTileset::compute_texture_region(uint32_t tile, Vec2I texture_size) const
  {
    auto layout = compute_layout(texture_size);
    return compute_texture_region({ static_cast<int32_t>(tile) % layout.w, static_cast<int32_t>(tile) / layout.w }, texture_size);
  }

  RectF MapTileset::compute_texture_region(Vec2I position, Vec2I texture_size) const
  {
    [[maybe_unused]] auto layout = compute_layout(texture_size);
    assert(position.x < layout.w);
    assert(position.y < layout.h);
    const RectI texture_rectangle = RectI::from_position_size(position * tile_size + position * spacing + margin, tile_size);
    const Vec2F texture_size_f = texture_size;
    return RectF::from_position_size(texture_rectangle.offset / texture_size_f, texture_rectangle.extent / texture_size_f);
  }

  const MapTilesetTile* MapTileset::tile(uint32_t id) const
  {
    auto [first, last] = std::equal_range(tiles.begin(), tiles.end(), id, MapTilesetTileComparator());

    if (std::distance(first, last) != 1) {
      return nullptr;
    }

    return &*first;
  }

  /*
   * TiledMap
   */

  TiledMap::TiledMap(const std::filesystem::path& filename)
  {
    std::ifstream file(filename);

    if (!file) {
      Log::fatal("Unknown TMX file: '{}'.", filename);
    }

    pugi::xml_document doc;
    const pugi::xml_parse_result result = doc.load(file);

    if (!result) {
      Log::fatal("Could not load TMX file '{}': {}.", filename, result.description());
    }

    parse_tmx_map(doc.child("map"), *this, filename.parent_path());
  }

  const MapTileset* TiledMap::tileset_from_gid(uint32_t gid) const noexcept
  {
    for (auto it = tilesets.rbegin(); it != tilesets.rend(); ++it) {
      if (it->first_gid <= gid) {
        return &(*it);
      }
    }

    return nullptr;
  }

  const std::vector<MapLayerStructure>& TiledMap::compute_structure(std::string_view path) const
  {
    auto path_elements = split_path(path);
    const auto* structure = &layers;

    for (auto layer_name : path_elements) {
      auto predicate = [this, layer_name](const MapLayerStructure& current) {
        if (current.type != MapLayerType::Group) {
          return false;
        }

        return group_layers[current.layer_index].layer.name == layer_name;
      };

      if (auto iterator = std::find_if(structure->begin(), structure->end(), predicate); iterator != structure->end()) {
        structure = &group_layers[iterator->layer_index].sub_layers;
      } else {
        Log::fatal("Unknown layer '{}' in path '{}'", layer_name, path);
      }
    }

    return *structure;
  }

}
