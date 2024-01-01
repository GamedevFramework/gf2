// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
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
  class RenderManager;
  class ResourceManager;
  class Texture;

  class GF_GRAPHICS_API Sprite {
  public:
    Sprite(const Texture* texture, RectF texture_region, Color color, RenderManager* render_manager);
    Sprite(const Texture* texture, RectF texture_region, RenderManager* render_manager);
    Sprite(const Texture* texture, RenderManager* render_manager);
    Sprite(const Texture* texture, const SpriteData& data, RenderManager* render_manager);
    Sprite(const SpriteResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    RenderGeometry geometry() const;

    const Texture* texture() const
    {
      return m_texture;
    }

    const Buffer* vertices() const
    {
      return &m_vertices;
    }

    const Buffer* indices() const
    {
      return &m_indices;
    }

    std::size_t index_count() const
    {
      return m_indices.size();
    }

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    const Texture* m_texture;
    Buffer m_vertices;
    Buffer m_indices;
    RectF m_bounds = {};
  };

}

#endif // GF_SPRITE_H
