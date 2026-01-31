// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_TILED_MAP_ASSETS_H
#define GF_TILED_MAP_ASSETS_H

#include <vector>

#include <gf2/core/TiledMap.h>
#include <gf2/core/TiledMapResource.h>

#include "GraphicsApi.h"

namespace gf {
  class GpuTexture;
  class RenderManager;
  class ResourceManager;
  class ResourceBundle;

  struct GF_GRAPHICS_API TiledMapAssetsContext {
    RenderManager* render_manager;
    ResourceManager* resource_manager;
  };

  class GF_GRAPHICS_API TiledMapAssets {
  public:
    using Context = TiledMapAssetsContext;
    using Primitive = TiledMap;

    TiledMapAssets() = default;
    TiledMapAssets(const TiledMap* map, std::vector<GpuTexture*> textures);
    TiledMapAssets(const TiledMapResource& resource, ResourceManager* resource_manager);
    TiledMapAssets(const std::filesystem::path& filename, TiledMapAssetsContext context);

    static ResourceBundle bundle(const std::filesystem::path& filename, TiledMapAssetsContext context);

    GpuTexture* texture(uint32_t i);
    const TiledMap* tiled_map() const;

  private:
    const TiledMap* m_tiled_map = nullptr;
    std::vector<GpuTexture*> m_textures;
  };

}

#endif // GF_TILED_MAP_ASSETS_H
