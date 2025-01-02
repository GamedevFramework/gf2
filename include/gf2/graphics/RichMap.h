// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_RICH_MAP_H
#define GF_RICH_MAP_H

#include <vector>

#include <gf2/core/RichMapResource.h>
#include <gf2/core/TiledMap.h>

#include "GraphicsApi.h"

namespace gf {
  class RenderManager;
  class ResourceManager;
  class ResourceBundle;
  class Texture;

  struct GF_GRAPHICS_API RichMapContext {
    RenderManager* render_manager;
    ResourceManager* resource_manager;
  };

  class GF_GRAPHICS_API RichMap {
  public:
    using Context = RichMapContext;
    using Primitive = TiledMap;

    RichMap() = default;
    RichMap(const TiledMap* map, std::vector<const Texture*> textures);
    RichMap(const RichMapResource& resource, ResourceManager* resource_manager);
    RichMap(const std::filesystem::path& filename, RichMapContext context);

    static ResourceBundle bundle(const std::filesystem::path& filename, RichMapContext context);

    const Texture* texture(uint32_t i) const;
    const TiledMap* tiled_map() const;

  private:
    const TiledMap* m_tiled_map = nullptr;
    std::vector<const Texture*> m_textures;
  };

}

#endif // GF_RICH_MAP_H
