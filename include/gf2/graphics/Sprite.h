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

namespace gf {
  class Renderer;
  class Texture;

  class GF_GRAPHICS_API Sprite {
  public:
    Sprite(const Texture* texture, RectF texture_region, Color color, Renderer* renderer);
    Sprite(const Texture* texture, RectF texture_region, Renderer* renderer);
    Sprite(const Texture* texture, Renderer* renderer);
    Sprite(const Texture* texture, const SpriteData& data, Renderer* renderer);

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
      return m_indices.count();
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
