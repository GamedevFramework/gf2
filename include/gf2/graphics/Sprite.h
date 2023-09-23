// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SPRITE_H
#define GF_SPRITE_H

#include <gf2/core/Color.h>
#include <gf2/core/Rect.h>
#include <gf2/core/SpriteData.h>
#include <gf2/core/Transform.h>

#include "Buffer.h"

namespace gf {
  class Renderer;
  class Texture;

  class Sprite {
  public:
    Sprite(Texture* texture, RectF texture_region, Color color, Renderer* renderer);
    Sprite(Texture* texture, RectF texture_region, Renderer* renderer);
    Sprite(Texture* texture, Renderer* renderer);
    Sprite(Texture* texture, const SpriteData& data, Renderer* renderer);

    const Texture* texture() const
    {
      return m_texture;
    }

    const Buffer* vertices() const
    {
      return &m_vertices;
    }

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    Texture* m_texture;
    Buffer m_vertices;
    RectF m_bounds = {};
  };

}

#endif // GF_SPRITE_H
