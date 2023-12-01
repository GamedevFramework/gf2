// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TILED_MAP_H
#define GF_TILED_MAP_H

#include <filesystem>
#include <string_view>
#include <vector>

#include <gf2/core/AnyGrid.h>
#include <gf2/core/Array2D.h>
#include <gf2/core/Flags.h>
#include <gf2/core/Rect.h>
#include <gf2/core/TiledMapData.h>
#include <gf2/core/Vec2.h>

#include "Buffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class RenderManager;
  class ResourceBundle;
  class ResourceManager;
  class Texture;

  enum class TiledMapQuery : uint8_t {
    Tile = 0x01,
    Object = 0x02,
    Recursive = 0x04,
  };

  template<>
  struct EnableBitmaskOperators<TiledMapQuery> : std::true_type {
  };

  struct GF_GRAPHICS_API TiledMapContext {
    RenderManager* render_manager;
    ResourceManager* resource_manager;
  };

  class GF_GRAPHICS_API TiledMap {
  public:
    using Primitive = TiledMapResource;
    using Context = TiledMapContext;

    TiledMap(std::vector<const Texture*> textures, const TiledMapData& data, RenderManager* render_manager);
    TiledMap(const TiledMapResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);
    TiledMap(const std::filesystem::path& filename, TiledMapContext context);

#ifdef _MSC_VER
    // why?
    TiledMap(const TiledMap&) = delete;
    TiledMap(TiledMap&&) noexcept = default;
    ~TiledMap() = default;

    TiledMap& operator=(const TiledMap&) = delete;
    TiledMap& operator=(TiledMap&&) noexcept = default;
#endif

    static ResourceBundle bundle(const std::filesystem::path& filename, TiledMapContext context);

    std::vector<RenderGeometry> select_geometry(Vec2I position, std::string_view path, Flags<TiledMapQuery> query = All);
    std::vector<RenderGeometry> select_geometry(std::string_view path, Flags<TiledMapQuery> query = All);

    Vec2I compute_position(Vec2F location) const;

    const AnyGrid& grid() const
    {
      return m_grid;
    }

  private:
    void compute_grid(const TiledMapData& data);
    void compute_tile_layers(const TiledMapData& data, RenderManager* render_manager);
    void compute_object_layers(const TiledMapData& data, RenderManager* render_manager);

    const std::vector<LayerStructureData>& compute_structure(std::string_view path) const;

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

    void compute_geometries(RectI view, Flags<TiledMapQuery> query, const std::vector<LayerStructureData>& structure, std::vector<RenderGeometry>& geometries) const;
    void compute_tile_geometry(RectI view, const TileLayer& tile_layer, std::vector<RenderGeometry>& geometries) const;
    void compute_object_geometry(const ObjectLayer& object_layer, std::vector<RenderGeometry>& geometries) const;

    std::vector<const Texture*> m_textures;
    TiledMapData m_data;
    RectF m_bounds = RectF::from_size({ 0.0f, 0.0f });
    AnyGrid m_grid;
    std::vector<TileLayer> m_tile_layers;
    std::vector<ObjectLayer> m_object_layers;
  };

} // namespace gf

#endif // GF_TILED_MAP_H
