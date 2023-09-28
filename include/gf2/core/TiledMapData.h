// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TILED_MAP_DATA_H
#define GF_TILED_MAP_DATA_H

#include <cstdint>

#include <filesystem>
#include <string>
#include <variant>
#include <vector>

#include "Array2D.h"
#include "GridTypes.h"
#include "Id.h"
#include "Property.h"
#include "PropertyMap.h"
#include "TypeTraits.h"
#include "Vec2.h"

namespace gf {

  constexpr uint32_t NoIndex = 0xFFFFFFFF;

  struct TilesetData {
    uint32_t properties_index = NoIndex;
    uint32_t texture_index = NoIndex;
    uint32_t first_gid = 0;
    Vec2I tile_size = { 0, 0 };
    int32_t spacing = 0;
    int32_t margin = 0;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TilesetData, Archive>& data)
  {
    return ar | data.properties_index | data.texture_index | data.first_gid | data.tile_size | data.spacing | data.margin;
  }

  enum LayerType : uint8_t {
    Tile,
    Object,
    Group,
  };

  struct LayerData {
    uint32_t properties_index = NoIndex;
    std::string name;
    Vec2I offset = { 0, 0 };
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<LayerData, Archive>& data)
  {
    return ar | data.properties_index | data.name | data.offset;
  }

  struct TileData {
    uint32_t gid = 0;
    Flags<CellFlip> flip = None;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TileData, Archive>& data)
  {
    return ar | data.gid | data.flip;
  }

  struct TileLayerData {
    LayerData layer;
    Array2D<TileData> cells;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TileLayerData, Archive>& data)
  {
    return ar | data.layer | data.cells;
  }

  enum class ObjectType : uint8_t {
    Point,
    Rectangle,
    Ellipse,
    Tile,
    Polygon,
    Polyline,
  };

  struct ObjectData {
    uint32_t properties_index = NoIndex;
    ObjectType type = ObjectType::Point;
    Id id = InvalidId;
    Vec2F location = { 0.0f, 0.0f };
    float rotation = 0.0f;
    std::variant<std::monostate, Vec2F, TileData, std::vector<Vec2F>> feature;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<ObjectData, Archive>& data)
  {
    return ar | data.properties_index | data.type | data.id | data.location | data.rotation | data.feature;
  }

  struct ObjectLayerData {
    LayerData layer;
    std::vector<ObjectData> objects;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<ObjectLayerData, Archive>& data)
  {
    return ar | data.layer | data.objects;
  }

  struct LayerStructureData {
    LayerType type;
    std::variant<uint32_t, std::vector<LayerStructureData>> node;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<LayerStructureData, Archive>& data)
  {
    return ar | data.type | data.node;
  }

  struct TiledMapData {
    std::vector<PropertyMap> properties;
    std::vector<TilesetData> tilesets;

    std::vector<TileLayerData> tile_layers;
    std::vector<ObjectLayerData> object_layers;

    std::vector<LayerStructureData> layers;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TiledMapData, Archive>& data)
  {
    return ar | data.properties | data.tilesets | data.tile_layers | data.object_layers | data.layers;
  }

  struct TiledMapResource {
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
