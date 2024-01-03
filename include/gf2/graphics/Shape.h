// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SHAPE_H
#define GF_SHAPE_H

#include <optional>

#include <gf2/core/Rect.h>
#include <gf2/core/ShapeData.h>

#include "Buffer.h"
#include "DynamicBuffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class RenderManager;
  class Texture;

  struct GF_GRAPHICS_API ShapeGeometry {
    RenderGeometry shape;
    std::optional<RenderGeometry> outline;
  };

  class GF_GRAPHICS_API Shape {
  public:
    Shape(const Texture* texture, const ShapeData& data, RenderManager* render_manager);

    ShapeGeometry geometry() const;

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    const Texture* m_texture = nullptr;
    Buffer m_vertices;
    Buffer m_indices;
    std::optional<Buffer> m_outline_vertices;
    std::optional<Buffer> m_outline_indices;
    RectF m_bounds = {};
  };

  class GF_GRAPHICS_API ShapeGroup {
  public:
    ShapeGroup();
    ShapeGroup(const ShapeGroupData& data, RenderManager* render_manager);

    void update(const ShapeGroupData& data, RenderManager* render_manager);

    RenderGeometry geometry() const;

  private:
    DynamicBuffer m_vertices;
    DynamicBuffer m_indices;
  };

}

#endif // GF_SHAPE_H
