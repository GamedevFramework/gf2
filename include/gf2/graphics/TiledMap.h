// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TILED_MAP_H
#define GF_TILED_MAP_H

#include <string_view>

#include <gf2/core/AnyGrid.h>
#include <gf2/core/Array2D.h>
#include <gf2/core/Flags.h>
#include <gf2/core/Rect.h>
#include <gf2/core/Vec2.h>

#include "Buffer.h"
#include "GraphicsApi.h"

namespace gf {
  class Renderer;
  class Texture;
  struct TiledMapData;
  struct TileData;

  enum class TiledMapQuery : uint8_t {
    Tile = 0x01,
    Object = 0x02,
    Recursive = 0x04,
  };

  template<>
  struct EnableBitmaskOperators<TiledMapQuery> : std::true_type {
  };

  class GF_GRAPHICS_API TiledMap {
  public:
    TiledMap(std::vector<const Texture*> textures, const TiledMapData& data, Renderer* renderer);
#ifdef _MSC_VER
    // why?
    TiledMap(const TiledMap&) = delete;
    TiledMap(TiledMap&&) noexcept = default;
    ~TiledMap() = default;

    TiledMap& operator=(const TiledMap&) = delete;
    TiledMap& operator=(TiledMap&&) noexcept = default;
#endif

    void select_group(Vec2I position, std::string_view path, Flags<TiledMapQuery> query = All);

  private:
    void compute_grid(const TiledMapData& data);
    void compute_tile_layers(const TiledMapData& data, Renderer* renderer);
    void compute_object_layers(const TiledMapData& data, Renderer* renderer);

    struct BufferRange {
      std::size_t texture_index = 0;
      std::size_t first = 0;
      std::size_t count = 0;
    };

    struct LayerBuffers {
      std::vector<BufferRange> ranges;
      Buffer vertices;
      Buffer indices;
    };

    struct TileLayer {
      Array2D<LayerBuffers> chunks;
    };

    struct ObjectLayer {
      LayerBuffers buffers;
    };

    std::vector<const Texture*> m_textures;
    RectF m_bounds = RectF::from_size({ 0.0f, 0.0f });
    AnyGrid m_grid;
    std::vector<TileLayer> m_tile_layers;
    std::vector<ObjectLayer> m_object_layers;
  };

} // namespace gf

#endif // GF_TILED_MAP_H
