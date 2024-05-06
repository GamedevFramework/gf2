// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/Sprite.h>

#include <gf2/core/ResourceManager.h>

#include <gf2/graphics/CommandBuffer.h>
#include <gf2/graphics/RenderManager.h>
#include <gf2/graphics/Texture.h>
#include <gf2/graphics/Vertex.h>

namespace gf {

  Sprite::Sprite(const Texture* texture, RectF texture_region, Color color, RenderManager* render_manager)
  : m_texture(texture)
  {
    const Vec2F size = texture->size() * texture_region.size();
    m_bounds = RectF::from_size(size);
    color = srgb_to_linear(color);

    Vertex vertices[4] = {
      { m_bounds.position_at(Orientation::NorthEast), texture_region.position_at(Orientation::NorthEast), color },
      { m_bounds.position_at(Orientation::SouthEast), texture_region.position_at(Orientation::SouthEast), color },
      { m_bounds.position_at(Orientation::NorthWest), texture_region.position_at(Orientation::NorthWest), color },
      { m_bounds.position_at(Orientation::SouthWest), texture_region.position_at(Orientation::SouthWest), color },
    };

    m_vertices = Buffer(BufferType::Device, BufferUsage::Vertex, std::begin(vertices), std::size(vertices), render_manager);

    // clang-format off
    const uint16_t indices[] = {
      0, 1, 2, // first triangle
      2, 1, 3, // second triangle
    };
    // clang-format on

    m_indices = Buffer(BufferType::Device, BufferUsage::Index, std::begin(indices), std::size(indices), render_manager);
  }

  Sprite::Sprite(const Texture* texture, RectF texture_region, RenderManager* render_manager)
  : Sprite(texture, texture_region, White, render_manager)
  {
  }

  Sprite::Sprite(const Texture* texture, RenderManager* render_manager)
  : Sprite(texture, RectF::from_size({ 1.0f, 1.0f }), White, render_manager)
  {
  }

  Sprite::Sprite(const Texture* texture, const SpriteData& data, RenderManager* render_manager)
  : Sprite(texture, data.texture_region, data.color, render_manager)
  {
  }

  Sprite::Sprite(const SpriteResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : Sprite(resource_manager->get<Texture>(resource.texture), resource.data, render_manager)
  {
  }

  RenderGeometry Sprite::geometry() const
  {
    RenderGeometry geometry = {};
    geometry.vertices = &m_vertices;
    geometry.indices = &m_indices;
    geometry.size = m_indices.size();
    geometry.texture = m_texture;
    return geometry;
  }

}
