// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/NinePatch.h>

#include <gf2/core/ResourceManager.h>

#include <gf2/graphics/RenderManager.h>
#include <gf2/graphics/Texture.h>
#include <gf2/graphics/Vertex.h>

namespace gf {

  namespace {

    /*
     *  0---4---8--12
     *  |   |   |   |
     *  1---5---9--13
     *  |   |   |   |
     *  2---6--10--14
     *  |   |   |   |
     *  3---7--11--15
     *
     */

    // clang-format off
    constexpr uint16_t Indices[] = {
       0,  1,  4,
       1,  4,  5,
       4,  5,  8,
       5,  8,  9,
       8,  9, 12,
       9, 12, 13,
      // second row
       1,  2,  5,
       2,  5,  6,
       5,  6,  9,
       6,  9, 10,
       9, 10, 13,
      10, 13, 14,
      // third row
       2,  3,  6,
       3,  6,  7,
       6,  7, 10,
       7, 10, 11,
      10, 11, 14,
      11, 14, 15,
    };
    // clang-format on

  }

  NinePatch::NinePatch(const Texture* texture, Vec2F size, NinePatchLimits limits, RectF texture_region, Color color, RenderManager* render_manager)
  : m_texture(texture)
  , m_bounds(RectF::from_size(size))
  {
    color = srgb_to_linear(color);

    const Vec2F texture_size = m_texture->size();

    const float x0 = 0.0f;
    const float x1 = texture_size.w * limits.left;
    const float x2 = size.w - (texture_size.w * limits.right);
    const float x3 = size.w;

    const float y0 = 0.0f;
    const float y1 = texture_size.h * limits.top;
    const float y2 = size.h - (texture_size.h * limits.bottom);
    const float y3 = size.h;

    const float u0 = texture_region.offset.x;
    const float u1 = texture_region.offset.x + (limits.left * texture_region.extent.w);
    const float u2 = texture_region.offset.x + ((1.0f - limits.right) * texture_region.extent.w);
    const float u3 = texture_region.offset.x + texture_region.extent.w;

    const float v0 = texture_region.offset.y;
    const float v1 = texture_region.offset.y + (limits.top * texture_region.extent.h);
    const float v2 = texture_region.offset.y + ((1.0f - limits.bottom) * texture_region.extent.h);
    const float v3 = texture_region.offset.y + texture_region.extent.h;

    const Vertex vertices[] = {
      { { x0, y0 }, { u0, v0 }, color },
      { { x0, y1 }, { u0, v1 }, color },
      { { x0, y2 }, { u0, v2 }, color },
      { { x0, y3 }, { u0, v3 }, color },
      { { x1, y0 }, { u1, v0 }, color },
      { { x1, y1 }, { u1, v1 }, color },
      { { x1, y2 }, { u1, v2 }, color },
      { { x1, y3 }, { u1, v3 }, color },
      { { x2, y0 }, { u2, v0 }, color },
      { { x2, y1 }, { u2, v1 }, color },
      { { x2, y2 }, { u2, v2 }, color },
      { { x2, y3 }, { u2, v3 }, color },
      { { x3, y0 }, { u3, v0 }, color },
      { { x3, y1 }, { u3, v1 }, color },
      { { x3, y2 }, { u3, v2 }, color },
      { { x3, y3 }, { u3, v3 }, color },
    };

    m_vertices = Buffer(BufferType::Device, BufferUsage::Vertex, std::begin(vertices), std::size(vertices), render_manager);
    m_indices = Buffer(BufferType::Device, BufferUsage::Index, std::begin(Indices), std::size(Indices), render_manager);
  }

  NinePatch::NinePatch(const Texture* texture, Vec2F size, NinePatchLimits limits, RectF texture_region, RenderManager* render_manager)
  : NinePatch(texture, size, limits, texture_region, White, render_manager)
  {
  }

  NinePatch::NinePatch(const Texture* texture, Vec2F size, NinePatchLimits limits, RenderManager* render_manager)
  : NinePatch(texture, size, limits, RectF::from_size({ 1.0f, 1.0f }), White, render_manager)
  {
  }

  NinePatch::NinePatch(const Texture* texture, const NinePatchData& data, RenderManager* render_manager)
  : NinePatch(texture, data.size, data.limits, data.texture_region, data.color, render_manager)
  {
  }

  NinePatch::NinePatch(const NinePatchResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : NinePatch(resource_manager->get<Texture>(resource.texture), resource.data, render_manager)
  {
  }

  RenderGeometry NinePatch::geometry() const
  {
    RenderGeometry geometry = {};
    geometry.vertices = &m_vertices;
    geometry.indices = &m_indices;
    geometry.size = m_indices.size();
    geometry.texture = m_texture;
    return geometry;
  }

}
