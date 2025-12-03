// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SPRITE_GROUP_H
#define GF_SPRITE_GROUP_H

#include <vector>

#include <gf2/core/SpriteData.h>

#include "DynamicBuffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class GpuTexture;
  class RenderManager;
  class ResourceManager;

  class GF_GRAPHICS_API SpriteGroup {
  public:
    SpriteGroup(std::vector<const GpuTexture*> textures, const SpriteGroupData& data, RenderManager* render_manager);
    SpriteGroup(const SpriteGroupResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    std::vector<RenderGeometry> geometry();

    void update(const SpriteGroupData& data, RenderManager* render_manager);

  private:
    std::vector<const GpuTexture*> m_textures;
    DynamicBuffer m_vertices;
    DynamicBuffer m_indices;

    struct BufferRange {
      uint32_t texture_index = 0;
      std::size_t first = 0;
      std::size_t size = 0;
    };

    std::vector<BufferRange> m_ranges;
  };

}

#endif // GF_SPRITE_GROUP_H
