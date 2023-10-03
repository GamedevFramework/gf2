// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/core/TiledMapData.h>
// clang-format on

#include <array>
#include <charconv>
#include <fstream>
#include <optional>
#include <stdexcept>

#include <fmt/std.h>
#include <pugixml.hpp>
#include <zlib.h>

#include <gf2/core/Log.h>
#include <gf2/core/Property.h>
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
              property_map.add_property(std::move(name), gf::hash(value_attribute.as_string()));
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

    uint32_t parse_tmx_properties(const pugi::xml_node node, TiledMapResource& resource)
    {
      PropertyMap property_map = parse_tmx_raw_properties(node);

      if (property_map.empty()) {
        return NoIndex;
      }

      auto index = static_cast<uint32_t>(resource.data.properties.size());
      resource.data.properties.push_back(std::move(property_map));
      return index;
    }

    /*
     * layers
     */

    LayerData parse_tmx_layer_common(const pugi::xml_node node, TiledMapResource& resource)
    {
      LayerData data;
      data.properties_index = parse_tmx_properties(node, resource);
      data.name = node.attribute("name").as_string();
      data.offset.x = node.attribute("offsetx").as_int(0);
      data.offset.y = node.attribute("offsety").as_int(0);
      unsupported_attribute(node, "opacity");
      unsupported_attribute(node, "visible");
      return data;
    }

    // tile layer

    enum class TmxFormat : uint8_t {
      Xml,
      Base64,
      Base64_Zlib,
      Base64_Gzip,
      Csv,
    };

    // http://en.wikibooks.org/wiki/Algorithm_implementation/Miscellaneous/Base64
    std::vector<uint8_t> parse_data_base64(std::string input)
    {
      input.erase(std::remove_if(input.begin(), input.end(), [](char c) { return c == '\n' || c == ' '; }), input.end());
      const std::size_t len = input.size();
      assert(len % 4 == 0);
      size_t padding = 0;

      if (len >= 2) {
        if (input[len - 1] == '=') {
          ++padding;
        }

        if (input[len - 2] == '=') {
          ++padding;
        }
      }

      // Setup a vector to hold the result
      std::vector<uint8_t> decoded;
      decoded.reserve(((len / 4) * 3) - padding);

      for (std::size_t i = 0; i < len;) {

        uint32_t tmp = 0; // Holds decoded quanta
        for (int k = 0; k < 4; ++k) {
          tmp <<= 6;
          char c = input[i];

          if (c >= 'A' && c <= 'Z') {
            tmp |= c - 'A';
          } else if (c >= 'a' && c <= 'z') {
            tmp |= c - 'a' + 26;
          } else if (c >= '0' && c <= '9') {
            tmp |= c - '0' + 52;
          } else if (c == '+') {
            tmp |= 0x3E;
          } else if (c == '/') {
            tmp |= 0x3F;
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
            Log::error("Unknown character: '{}' ({#X}).", c, c);
            assert(false);
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
      assert(node.name() == "data"sv);

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

    TileData parse_gid(uint32_t gid)
    {
      static constexpr uint32_t FlippedHorizontallyBit = UINT32_C(0x80000000);
      static constexpr uint32_t FlippedVerticallyBit = UINT32_C(0x40000000);
      static constexpr uint32_t FlippedDiagonallyBit = UINT32_C(0x20000000);
      static constexpr uint32_t RotatedHexagonal120Bit = UINT32_C(0x10000000);

      TileData tile;

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

    std::vector<TileData> parse_tmx_tiles(const pugi::xml_node node, TmxFormat format)
    {
      std::vector<TileData> tiles;

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

    LayerStructureData parse_tmx_tile_layer(const pugi::xml_node node, TiledMapResource& resource)
    {
      assert(node.name() == "layer"sv);

      TileLayerData tile_layer;
      tile_layer.layer = parse_tmx_layer_common(node, resource);

      for (const pugi::xml_node data_node : node.children("data")) {
        unsupported_node(data_node, "chunk"); // TODO
        auto format = parse_data_format(data_node);
        auto tiles = parse_tmx_tiles(data_node, format);
        assert(tiles.size() == static_cast<std::size_t>(resource.data.map_size.w * resource.data.map_size.h));
        tile_layer.tiles = Array2D<TileData>(resource.data.map_size);

        for (std::size_t i = 0; i < tiles.size(); ++i) {
          tile_layer.tiles[i] = tiles[i];
        }
      }

      LayerStructureData data = {};
      data.type = LayerType::Tile;
      data.layer_index = static_cast<uint32_t>(resource.data.tile_layers.size());

      resource.data.tile_layers.push_back(std::move(tile_layer));

      return data;
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

    ObjectData parse_tmx_object(const pugi::xml_node node, TiledMapResource& resource)
    {
      ObjectData object;
      object.properties_index = parse_tmx_properties(node, resource);
      object.id = gf::id(node.attribute("id").as_int());
      object.name = node.attribute("name").as_string();
      object.location.x = node.attribute("x").as_float();
      object.location.y = node.attribute("y").as_float();
      object.rotation = node.attribute("rotation").as_float();

      unsupported_attribute(node, "type"); // TODO?
      unsupported_attribute(node, "visible");

      if (const pugi::xml_node polygon = node.child("polygon"); polygon) {
        object.type = ObjectType::Polygon;
        object.feature = parse_points(polygon.attribute("points").as_string());
      } else if (const pugi::xml_node polyline = node.child("polyline"); polyline) {
        object.type = ObjectType::Polyline;
        object.feature = parse_points(polyline.attribute("points").as_string());
      } else if (!node.child("text").empty()) {
        unsupported_node(node, "text");
      } else if (const pugi::xml_attribute gid = node.attribute("gid"); gid) {
        object.type = ObjectType::Tile;
        object.feature = parse_gid(gid.as_uint());
      } else if (!node.child("point").empty()) {
        object.type = ObjectType::Point;
      } else if (!node.child("ellipse").empty()) {
        object.type = ObjectType::Ellipse;
        const Vec2F size = { node.attribute("width").as_float(), node.attribute("height").as_float() };
        object.feature = size;
      } else {
        object.type = ObjectType::Rectangle;
        const Vec2F size = { node.attribute("width").as_float(), node.attribute("height").as_float() };
        object.feature = size;
      }

      return object;
    }

    LayerStructureData parse_tmx_object_layer(const pugi::xml_node node, TiledMapResource& resource)
    {
      assert(node.name() == "objectgroup"sv);

      ObjectLayerData object_layer;
      object_layer.layer = parse_tmx_layer_common(node, resource);

      unsupported_attribute(node, "color");
      unsupported_attribute(node, "draworder");

      for (const pugi::xml_node object : node.children("object")) {
        object_layer.objects.push_back(parse_tmx_object(object, resource));
      }

      LayerStructureData data = {};
      data.type = LayerType::Object;
      data.layer_index = static_cast<uint32_t>(resource.data.object_layers.size());

      resource.data.object_layers.push_back(std::move(object_layer));

      return data;
    }

    // group layer

    std::vector<LayerStructureData> parse_tmx_layer_structure(pugi::xml_node node, TiledMapResource& resource);

    // NOLINTNEXTLINE(misc-no-recursion)
    LayerStructureData parse_tmx_group_layer(const pugi::xml_node node, TiledMapResource& resource)
    {
      assert(node.name() == "group"sv);

      GroupLayerData group_layer = {};
      group_layer.layer = parse_tmx_layer_common(node, resource);
      group_layer.sub_layers = parse_tmx_layer_structure(node, resource);

      LayerStructureData data = {};
      data.type = LayerType::Group;
      data.layer_index = static_cast<uint32_t>(resource.data.group_layers.size());

      resource.data.group_layers.push_back(std::move(group_layer));

      return data;
    }

    // NOLINTNEXTLINE(misc-no-recursion)
    std::vector<LayerStructureData> parse_tmx_layer_structure(const pugi::xml_node node, TiledMapResource& resource)
    {
      std::vector<LayerStructureData> layers;

      for (const pugi::xml_node layer : node.children()) {
        const std::string name = layer.name();

        if (name == "layer") {
          layers.push_back(parse_tmx_tile_layer(node, resource));
        } else if (name == "objectgroup") {
          layers.push_back(parse_tmx_object_layer(node, resource));
        } else if (name == "group") {
          layers.push_back(parse_tmx_group_layer(node, resource));
        }
      }

      return layers;
    }

    /*
     * tileset
     */

    TilesetTileData parse_tmx_tileset_tile(const pugi::xml_node node, TiledMapResource& resource)
    {
      assert(node.name() == "tile"sv);
      TilesetTileData tile;
      tile.properties_index = parse_tmx_properties(node, resource);
      tile.id = node.attribute("id").as_int();
      tile.type = node.attribute("type").as_string();

      const pugi::xml_node objects = node.child("objectgroup");

      if (!objects.empty()) {
        tile.objects = parse_tmx_object_layer(objects, resource).layer_index;
      }

      unsupported_node(node, "image");
      unsupported_node(node, "animation");

      return tile;
    }

    void parse_tmx_tileset_from_element(TilesetData& tileset, const pugi::xml_node node, TiledMapResource& resource, const std::filesystem::path& base_directory)
    {
      assert(node.name() == "tileset"sv);

      tileset.tile_size.w = node.attribute("tilewidth").as_int();
      tileset.tile_size.h = node.attribute("tileheight").as_int();
      tileset.spacing = node.attribute("spacing").as_int();
      tileset.margin = node.attribute("margin").as_int();

      const pugi::xml_node image = node.child("image");
      assert(image);
      std::filesystem::path image_path = base_directory / image.attribute("source").as_string();
      tileset.texture_index = static_cast<uint32_t>(resource.textures.size());
      resource.textures.push_back(std::move(image_path));

      for (const pugi::xml_node tile : node.children("tile")) {
        tileset.tiles.push_back(parse_tmx_tileset_tile(tile, resource));
      }

      unsupported_node(node, "tileoffset");
      unsupported_node(node, "wangsets");
    }

    void parse_tmx_tileset_from_file(TilesetData& tileset, const std::filesystem::path& source, TiledMapResource& resource, const std::filesystem::path& base_directory)
    {
      std::filesystem::path tileset_path = base_directory / source;
      std::ifstream tileset_file(tileset_path);

      if (!tileset_file) {
        Log::error("Unknown TSX file: '{}'.", tileset_path);
        throw std::runtime_error("Unknown TSX file");
      }

      pugi::xml_document doc;
      const pugi::xml_parse_result result = doc.load(tileset_file);

      if (!result) {
        Log::error("Could not load TSX file '{}': {}.", tileset_path, result.description());
        throw std::runtime_error("Could not load TSX file");
      }

      const pugi::xml_node node = doc.child("tileset");

      if (!node.attribute("firstgid").empty()) {
        Log::warning("Attribute 'firstgid' present in a TSX file: '{}'.", tileset_path);
      }

      if (!node.attribute("source").empty()) {
        Log::warning("Attribute 'source' present in a TSX file: '{}'.", tileset_path);
      }

      parse_tmx_tileset_from_element(tileset, node, resource, tileset_path.parent_path());
    }

    TilesetData parse_tmx_tileset(const pugi::xml_node node, TiledMapResource& resource, const std::filesystem::path& base_directory)
    {
      assert(node.name() == "tileset"sv);
      TilesetData tileset;
      tileset.properties_index = parse_tmx_properties(node, resource);
      tileset.first_gid = node.attribute("firstgid").as_uint();

      const std::filesystem::path source = node.attribute("source").as_string();

      if (!source.empty()) {
        parse_tmx_tileset_from_file(tileset, source, resource, base_directory);
      } else {
        parse_tmx_tileset_from_element(tileset, node, resource, base_directory);
      }

      return tileset;
    }

    /*
     * map
     */

    void parse_tmx_map(const pugi::xml_node node, TiledMapResource& resource, const std::filesystem::path& base_directory)
    {
      assert(node.name() == "map"sv);

      resource.data.properties_index = parse_tmx_properties(node, resource);

      std::string orientation = node.attribute("orientation").as_string();
      assert(!orientation.empty());

      if (orientation == "orthogonal") {
        resource.data.orientation = GridOrientation::Orthogonal;
      } else if (orientation == "isometric") {
        resource.data.orientation = GridOrientation::Isometric;
      } else if (orientation == "staggered") {
        resource.data.orientation = GridOrientation::Staggered;
      } else if (orientation == "hexagonal") {
        resource.data.orientation = GridOrientation::Hexagonal;
      } else {
        Log::error("Wrong orientation string: '{}'.", orientation);
      }

      resource.data.map_size.w = node.attribute("width").as_int();
      resource.data.map_size.h = node.attribute("height").as_int();

      resource.data.tile_size.w = node.attribute("tilewidth").as_int();
      resource.data.tile_size.h = node.attribute("tileheight").as_int();

      resource.data.hex_side_length = node.attribute("hexsidelength").as_int(0);

      std::string cell_axis = node.attribute("staggeraxis").as_string("x");

      if (cell_axis == "x") {
        resource.data.cell_axis = CellAxis::X;
      } else if (cell_axis == "y") {
        resource.data.cell_axis = CellAxis::Y;
      } else {
        Log::error("Wrong cell axis string: '{}'.", cell_axis);
      }

      std::string cell_index = node.attribute("staggerindex").as_string("odd");

      if (cell_index == "odd") {
        resource.data.cell_index = CellIndex::Odd;
      } else if (cell_index == "even") {
        resource.data.cell_index = CellIndex::Even;
      } else {
        Log::error("Wrong cell index string: '{}'.", cell_index);
      }

      unsupported_attribute(node, "backgroundcolor");

      for (const pugi::xml_node tileset : node.children("tileset")) {
        resource.data.tilesets.push_back(parse_tmx_tileset(tileset, resource, base_directory));
      }

      resource.data.layers = parse_tmx_layer_structure(node, resource);
    }

  }

  /*
   * TilesetData
   */

  Vec2I TilesetData::compute_layout(Vec2I texture_size) const
  {
    if (tile_size.w == 0 || tile_size.h == 0) {
      return { 0, 0 };
    }

    return (texture_size - 2 * margin + spacing) / (tile_size + spacing);
  }

  RectF TilesetData::compute_texture_region(uint32_t tile, Vec2I texture_size) const
  {
    auto layout = compute_layout(texture_size);
    return compute_texture_region({ static_cast<int32_t>(tile) % layout.w, static_cast<int32_t>(tile) / layout.w }, texture_size);
  }

  RectF TilesetData::compute_texture_region(Vec2I position, Vec2I texture_size) const
  {
    [[maybe_unused]] auto layout = compute_layout(texture_size);
    assert(position.x < layout.w);
    assert(position.y < layout.h);
    const RectI texture_rectangle = RectI::from_position_size(position * tile_size + position * spacing + margin, tile_size);
    const Vec2F texture_size_f = texture_size;
    return RectF::from_position_size(texture_rectangle.offset / texture_size_f, texture_rectangle.extent / texture_size_f);
  }

  /*
   * TiledMapData
   */

  const TilesetData* TiledMapData::tileset_from_gid(uint32_t gid) const noexcept
  {
    for (auto it = tilesets.rbegin(); it != tilesets.rend(); ++it) {
      if (it->first_gid <= gid) {
        return &(*it);
      }
    }

    return nullptr;
  }

  /*
   * TiledMapResource
   */

  TiledMapResource::TiledMapResource(const std::filesystem::path& filename)
  {
    std::ifstream file(filename);

    if (!file) {
      Log::error("Unknown TMX file: '{}'.", filename);
      throw std::runtime_error("Unknown TMX file");
    }

    pugi::xml_document doc;
    const pugi::xml_parse_result result = doc.load(file);

    if (!result) {
      Log::error("Could not load TMX file '{}': {}.", filename, result.description());
      throw std::runtime_error("Could not load TMX file");
    }

    parse_tmx_map(doc.child("map"), *this, filename.parent_path());
  }

}
