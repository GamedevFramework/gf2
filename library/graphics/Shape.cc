// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Shape.h>
// clang-format on

#include <cassert>

namespace gf {

  Shape::Shape(Texture* texture, const ShapeData& data, Renderer* renderer)
  : m_texture(texture)
  {
  }

  ShapeGeometry Shape::geometry() const
  {
    ShapeGeometry geometry;

    RenderGeometry shape;
    shape.texture = m_texture;
    shape.vertices = &m_vertices;
    shape.indices = &m_indices;
    shape.count = m_indices.count();

    geometry.shape = shape;

    if (m_outline_vertices.has_value()) {
      assert(m_outline_indices.has_value());

      RenderGeometry outline;
      outline.pipeline = RenderPipelineType::Text;
      outline.texture = nullptr;
      outline.vertices = &m_outline_vertices.value();
      outline.indices = &m_outline_indices.value();
      outline.count = m_outline_indices.value().count();

      geometry.outline = outline;
    }

    return geometry;
  }

}
