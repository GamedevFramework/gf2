// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TILED_MAP_DATA_H
#define GF_TILED_MAP_DATA_H

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

  enum LayerType : uint8_t {
    Tile,
    Object,
    Group,
  };

  struct GF_CORE_API LayerData {
    uint32_t properties_index = NoIndex;
    std::string name;
    Vec2F offset = { 0.0f, 0.0f };
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<LayerData, Archive>& data)
  {
    return ar | data.properties_index | data.name | data.offset;
  }

  struct GF_CORE_API TileData {
    uint32_t gid = 0;
    Flags<CellFlip> flip = None;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TileData, Archive>& data)
  {
    return ar | data.gid | data.flip;
  }

  struct GF_CORE_API TileLayerData {
    LayerData layer;
    Array2D<TileData> tiles;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TileLayerData, Archive>& data)
  {
    return ar | data.layer | data.tiles;
  }

  enum class ObjectType : uint8_t {
    Point,
    Rectangle,
    Ellipse,
    Tile,
    Polygon,
    Polyline,
  };

  struct GF_CORE_API ObjectData {
    uint32_t properties_index = NoIndex;
    ObjectType type = ObjectType::Point;
    Id id = InvalidId;
    std::string name;
    Vec2F location = { 0.0f, 0.0f };
    float rotation = 0.0f;
    std::variant<std::monostate, Vec2F, TileData, std::vector<Vec2F>> feature;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<ObjectData, Archive>& data)
  {
    return ar | data.properties_index | data.type | data.id | data.name | data.location | data.rotation | data.feature;
  }

  struct GF_CORE_API ObjectLayerData {
    LayerData layer;
    std::vector<ObjectData> objects;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<ObjectLayerData, Archive>& data)
  {
    return ar | data.layer | data.objects;
  }

  struct GF_CORE_API LayerStructureData {
    LayerType type;
    uint32_t layer_index;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<LayerStructureData, Archive>& data)
  {
    return ar | data.type | data.layer_index;
  }

  struct GF_CORE_API GroupLayerData {
    LayerData layer;
    std::vector<LayerStructureData> sub_layers;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<GroupLayerData, Archive>& data)
  {
    return ar | data.layer | data.layers;
  }

  struct GF_CORE_API TilesetTileData {
    uint32_t properties_index = NoIndex;
    int32_t id = 0;
    std::string type;
    std::optional<uint32_t> objects = std::nullopt;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TilesetTileData, Archive>& data)
  {
    return ar | data.properties_index | data.id | data.type | data.objects;
  }

  struct GF_CORE_API TilesetData {
    uint32_t properties_index = NoIndex;
    uint32_t texture_index = NoIndex;
    uint32_t first_gid = 0;
    Vec2I tile_size = { 0, 0 };
    Vec2I offset = { 0, 0 };
    int32_t spacing = 0;
    int32_t margin = 0;
    std::vector<TilesetTileData> tiles;

    Vec2I compute_layout(Vec2I texture_size) const;
    RectF compute_texture_region(uint32_t tile, Vec2I texture_size) const;
    RectF compute_texture_region(Vec2I position, Vec2I texture_size) const;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TilesetData, Archive>& data)
  {
    return ar | data.properties_index | data.texture_index | data.first_gid | data.tile_size | data.spacing | data.margin | data.tiles;
  }

  struct GF_CORE_API TiledMapData {
    uint32_t properties_index = NoIndex;
    GridOrientation orientation = GridOrientation::Unknown;
    CellAxis cell_axis = CellAxis::X;
    CellIndex cell_index = CellIndex::Odd;
    int32_t hex_side_length = 0;

    Vec2I map_size = { 0, 0 };
    Vec2I tile_size = { 0, 0 };

    std::vector<PropertyMap> properties;
    std::vector<TilesetData> tilesets;

    std::vector<TileLayerData> tile_layers;
    std::vector<ObjectLayerData> object_layers;
    std::vector<GroupLayerData> group_layers;

    std::vector<LayerStructureData> layers;

    const TilesetData* tileset_from_gid(uint32_t gid) const noexcept;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TiledMapData, Archive>& data)
  {
    return ar | data.properties_index | data.orientation | data.cell_axis | data.cell_index | data.hex_side_length | data.map_size | data.tile_size | data.properties | data.tilesets | data.tile_layers | data.object_layers | data.layers;
  }

  struct GF_CORE_API TiledMapResource {
    TiledMapResource() = default;
    TiledMapResource(const std::filesystem::path& filename);

    std::vector<std::filesystem::path> textures;
    TiledMapData data;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TiledMapResource, Archive>& resource)
  {
    return ar | resource.textures | resource.data;
  }

}

#endif // GF_TILED_MAP_DATA_H
