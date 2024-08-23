// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_RICH_MAP_RENDERER_H
#define GF_RICH_MAP_RENDERER_H

#include <string_view>
#include <vector>

#include <gf2/core/AnyGrid.h>
#include <gf2/core/Array2D.h>
#include <gf2/core/Flags.h>
#include <gf2/core/Rect.h>
#include <gf2/core/Vec2.h>

#include "Buffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"
#include "RichMap.h"

namespace gf {
  class RenderManager;

  enum class RichMapQuery : uint8_t {
    Tile = 0x01,
    Object = 0x02,
    Recursive = 0x04,
  };

  template<>
  struct EnableBitmaskOperators<RichMapQuery> : std::true_type {
  };

  class GF_GRAPHICS_API RichMapRenderer {
  public:
    RichMapRenderer(const RichMap* map, RenderManager* render_manager);
    RichMapRenderer(const RichMapResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    std::vector<RenderGeometry> select_geometry(Vec2I position, std::string_view path, Flags<RichMapQuery> query = All);
    std::vector<RenderGeometry> select_geometry(std::string_view path, Flags<RichMapQuery> query = All);

    std::vector<RenderGeometry> select_geometry(Vec2I position, const std::vector<MapLayerStructure>& structure, Flags<RichMapQuery> query = All);
    std::vector<RenderGeometry> select_geometry(const std::vector<MapLayerStructure>& structure, Flags<RichMapQuery> query = All);

    Vec2I compute_position(Vec2F location) const;

    const AnyGrid& grid() const
    {
      return m_grid;
    }

    const RichMap* rich_map() const
    {
      return m_map;
    }

    const TiledMap* tiled_map() const
    {
      return m_map->tiled_map();
    }

  private:
    void compute_tile_layers(RenderManager* render_manager);
    void compute_object_layers(RenderManager* render_manager);

    struct BufferRange {
      std::size_t texture_index = 0;
      std::size_t first = 0;
      std::size_t size = 0;
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

    void compute_geometries(RectI view, Flags<RichMapQuery> query, const std::vector<MapLayerStructure>& structure, std::vector<RenderGeometry>& geometries) const;
    void compute_tile_geometry(RectI view, const TileLayer& tile_layer, std::vector<RenderGeometry>& geometries) const;
    void compute_object_geometry(const ObjectLayer& object_layer, std::vector<RenderGeometry>& geometries) const;

    const RichMap* m_map;
    AnyGrid m_grid;
    std::vector<TileLayer> m_tile_layers;
    std::vector<ObjectLayer> m_object_layers;
  };

} // namespace gf

#endif // GF_RICH_MAP_RENDERER_H
