// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Sprite.h>
// clang-format on

#include <gf2/graphics/CommandBuffer.h>
#include <gf2/graphics/Renderer.h>
#include <gf2/graphics/Texture.h>
#include <gf2/graphics/Vertex.h>

namespace gf {

  Sprite::Sprite(Texture* texture, RectF texture_region, Color color, Renderer* renderer)
  : m_texture(texture)
  {
    const Vec2F size = texture->size() * texture_region.size();
    m_bounds = RectF::from_size(size);

    Vertex vertices[4] = {
      { m_bounds.position_at(Orientation::NorthEast), texture_region.position_at(Orientation::NorthEast), color },
      { m_bounds.position_at(Orientation::SouthEast), texture_region.position_at(Orientation::SouthEast), color },
      { m_bounds.position_at(Orientation::NorthWest), texture_region.position_at(Orientation::NorthWest), color },
      { m_bounds.position_at(Orientation::SouthWest), texture_region.position_at(Orientation::SouthWest), color },
    };

    m_vertices = Buffer(BufferType::Device, BufferUsage::Vertex, std::begin(vertices), std::size(vertices), renderer);
  }

  Sprite::Sprite(Texture* texture, RectF texture_region, Renderer* renderer)
  : Sprite(texture, texture_region, White, renderer)
  {
  }

  Sprite::Sprite(Texture* texture, Renderer* renderer)
  : Sprite(texture, RectF::from_size({ 1.0f, 1.0f }), White, renderer)
  {
  }

  Sprite::Sprite(Texture* texture, const SpriteData& data, Renderer* renderer)
  : Sprite(texture, data.texture_region, data.color, renderer)
  {
  }

}
