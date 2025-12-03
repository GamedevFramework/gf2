// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CURVE_H
#define GF_CURVE_H

#include <optional>

#include <gf2/core/CurveBuffer.h>
#include <gf2/core/Rect.h>

#include "DynamicBuffer.h"
#include "GpuBuffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class RenderManager;

  struct GF_GRAPHICS_API CurveGeometry {
    RenderGeometry curve;
    std::optional<RenderGeometry> outline;
  };

  class GF_GRAPHICS_API Curve {
  public:
    Curve(const CurveBuffer& buffer, RenderManager* render_manager);

    CurveGeometry geometry() const;

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    GpuBuffer m_vertices;
    GpuBuffer m_indices;
    std::optional<GpuBuffer> m_outline_vertices;
    std::optional<GpuBuffer> m_outline_indices;
    RectF m_bounds = {};
  };

  class GF_GRAPHICS_API CurveGroup {
  public:
    CurveGroup();
    CurveGroup(const CurveGroupBuffer& buffer, RenderManager* render_manager);

    void update(const CurveGroupBuffer& buffer, RenderManager* render_manager);

    RenderGeometry geometry() const;

  private:
    DynamicBuffer m_vertices;
    DynamicBuffer m_indices;
  };

}

#endif // GF_CURVE_H
