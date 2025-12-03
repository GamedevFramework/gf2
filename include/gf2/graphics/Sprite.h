// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SPRITE_H
#define GF_SPRITE_H

#include <gf2/core/Color.h>
#include <gf2/core/Rect.h>
#include <gf2/core/SpriteData.h>
#include <gf2/core/Transform.h>

#include "Buffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class GpuTexture;
  class RenderManager;
  class ResourceManager;

  class GF_GRAPHICS_API Sprite {
  public:
    Sprite(const GpuTexture* texture, RectF texture_region, Color color, RenderManager* render_manager);
    Sprite(const GpuTexture* texture, RectF texture_region, RenderManager* render_manager);
    Sprite(const GpuTexture* texture, RenderManager* render_manager);
    Sprite(const GpuTexture* texture, const SpriteData& data, RenderManager* render_manager);
    Sprite(const SpriteResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    RenderGeometry geometry() const;

    const GpuTexture* texture() const
    {
      return m_texture;
    }

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    const GpuTexture* m_texture;
    Buffer m_vertices;
    Buffer m_indices;
    RectF m_bounds = {};
  };

}

#endif // GF_SPRITE_H
