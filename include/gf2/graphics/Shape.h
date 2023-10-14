// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SHAPE_H
#define GF_SHAPE_H

#include <optional>

#include <gf2/core/ShapeData.h>

#include "Buffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class Renderer;
  class Texture;

  struct ShapeGeometry {
    RenderGeometry shape;
    std::optional<RenderGeometry> outline;
  };

  class GF_GRAPHICS_API Shape {
  public:
    Shape(Texture* texture, const ShapeData& data, Renderer* renderer);

    ShapeGeometry geometry() const;

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    Texture* m_texture = nullptr;
    Buffer m_vertices;
    Buffer m_indices;
    std::optional<Buffer> m_outline_vertices;
    std::optional<Buffer> m_outline_indices;
    RectF m_bounds = {};
  };

}

#endif // GF_SHAPE_H
