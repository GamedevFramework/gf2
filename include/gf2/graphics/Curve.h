// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CURVE_H
#define GF_CURVE_H

#include <optional>

#include <gf2/core/CurveData.h>
#include <gf2/core/Rect.h>

#include "Buffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class RenderManager;
  class Texture;

  struct GF_GRAPHICS_API CurveGeometry {
    RenderGeometry curve;
    std::optional<RenderGeometry> outline;
  };

  class GF_GRAPHICS_API Curve {
  public:
    Curve(const CurveData& data, RenderManager* render_manager);

    CurveGeometry geometry() const;

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    Buffer m_vertices;
    Buffer m_indices;
    std::optional<Buffer> m_outline_vertices;
    std::optional<Buffer> m_outline_indices;
    RectF m_bounds = {};
  };

  class GF_GRAPHICS_API CurveGroup {
  public:
    CurveGroup() = default;
    CurveGroup(const CurveGroupData& data, RenderManager* render_manager);

    void update(const CurveGroupData& data, RenderManager* render_manager);

    RenderGeometry geometry() const;

  private:
    static constexpr std::size_t FramesInFlight = 3;
    std::size_t m_current_buffer = 0;
    Buffer m_vertices[FramesInFlight];
    Buffer m_indices[FramesInFlight];
  };

}

#endif // GF_CURVE_H
