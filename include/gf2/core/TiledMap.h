// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_TILED_MAP_H
#define GF_TILED_MAP_H

#include <cstdint>

#include <filesystem>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "Array2D.h"
#include "CoreApi.h"
#include "GridTypes.h"
#include "Id.h"
#include "Property.h"
#include "PropertyMap.h"
#include "Rect.h"
#include "TypeTraits.h"
#include "Vec2.h"

namespace gf {

  constexpr uint32_t NoIndex = 0xFFFFFFFF;

  enum MapLayerType : uint8_t {
    Tile,
    Object,
    Group,
  };

  struct GF_CORE_API MapLayer {
    uint32_t properties_index = NoIndex;
    std::string name;
    std::string type;
    Vec2I offset = { 0, 0 };
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<MapLayer, Archive>& data)
  {
    return ar | data.properties_index | data.name | data.type | data.offset;
  }

  struct GF_CORE_API MapTile {
    uint32_t gid = 0;
    Flags<CellFlip> flip = None;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<MapTile, Archive>& data)
  {
    return ar | data.gid | data.flip;
  }

  struct GF_CORE_API MapTileLayer {
    MapLayer layer;
    Array2D<MapTile> tiles;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<MapTileLayer, Archive>& data)
  {
    return ar | data.layer | data.tiles;
  }

  enum class MapObjectType : uint8_t {
    Point,
    Rectangle,
    Ellipse,
    Tile,
    Polygon,
    Polyline,
  };

  struct GF_CORE_API MapObject {
    uint32_t properties_index = NoIndex;
    MapObjectType type = MapObjectType::Point;
    Id id = InvalidId;
    std::string name;
    Vec2F location = { 0.0f, 0.0f };
    float rotation = 0.0f;
    std::variant<std::monostate, Vec2F, MapTile, std::vector<Vec2F>> feature;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<MapObject, Archive>& data)
  {
    return ar | data.properties_index | data.type | data.id | data.name | data.location | data.rotation | data.feature;
  }

  struct GF_CORE_API MapObjectLayer {
    MapLayer layer;
    std::vector<MapObject> objects;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<MapObjectLayer, Archive>& data)
  {
    return ar | data.layer | data.objects;
  }

  struct GF_CORE_API MapLayerStructure {
    MapLayerType type;
    uint32_t layer_index;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<MapLayerStructure, Archive>& data)
  {
    return ar | data.type | data.layer_index;
  }

  struct GF_CORE_API MapGroupLayer {
    MapLayer layer;
    std::vector<MapLayerStructure> sub_layers;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<MapGroupLayer, Archive>& data)
  {
    return ar | data.layer | data.sub_layers;
  }

  struct GF_CORE_API MapTilesetTile {
    uint32_t properties_index = NoIndex;
    uint32_t id = 0;
    std::string type;
    std::optional<uint32_t> objects = std::nullopt;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<MapTilesetTile, Archive>& data)
  {
    return ar | data.properties_index | data.id | data.type | data.objects;
  }

  struct GF_CORE_API MapTileset {
    std::string type;
    uint32_t properties_index = NoIndex;
    uint32_t texture_index = NoIndex;
    uint32_t first_gid = 0;
    Vec2I tile_size = { 0, 0 };
    Vec2I offset = { 0, 0 };
    int32_t spacing = 0;
    int32_t margin = 0;
    std::vector<MapTilesetTile> tiles;

    Vec2I compute_layout(Vec2I texture_size) const;
    RectF compute_texture_region(uint32_t tile, Vec2I texture_size) const;
    RectF compute_texture_region(Vec2I position, Vec2I texture_size) const;
    const MapTilesetTile* tile(uint32_t id) const;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<MapTileset, Archive>& data)
  {
    return ar | data.type | data.properties_index | data.texture_index | data.first_gid | data.tile_size | data.spacing | data.margin | data.tiles;
  }

  struct GF_CORE_API TiledMap {
    std::string type;
    uint32_t properties_index = NoIndex;
    GridOrientation orientation = GridOrientation::Unknown;
    CellAxis cell_axis = CellAxis::X;
    CellIndex cell_index = CellIndex::Odd;
    int32_t hex_side_length = 0;

    Vec2I map_size = { 0, 0 };
    Vec2I tile_size = { 0, 0 };

    std::vector<PropertyMap> properties;
    std::vector<MapTileset> tilesets;

    std::vector<MapTileLayer> tile_layers;
    std::vector<MapObjectLayer> object_layers;
    std::vector<MapGroupLayer> group_layers;

    std::vector<MapLayerStructure> layers;

    std::vector<std::filesystem::path> textures;

    TiledMap() = default;
    TiledMap(const std::filesystem::path& filename);

    const MapTileset* tileset_from_gid(uint32_t gid) const noexcept;
    const std::vector<MapLayerStructure>& compute_structure(std::string_view path) const;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TiledMap, Archive>& map)
  {
    return ar | map.type | map.properties_index | map.orientation | map.cell_axis | map.cell_index | map.hex_side_length | map.map_size | map.tile_size | map.properties | map.tilesets | map.tile_layers | map.object_layers | map.group_layers | map.layers | map.textures;
  }

}

#endif // GF_TILED_MAP_H
