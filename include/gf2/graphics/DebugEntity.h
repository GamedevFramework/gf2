// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_DEBUG_ENTITY_H
#define GF_DEBUG_ENTITY_H

#include <gf2/core/Color.h>
#include <gf2/core/Span.h>
#include <gf2/core/Vec2.h>

#include "Curve.h"
#include "Entity.h"
#include "GraphicsApi.h"
#include "Shape.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API DebugEntity : public Entity {
  public:
    DebugEntity(RenderManager* render_manager);

    void start_debug();
    void stop_debug();

    void draw_circle(Vec2F center, float angle, float radius, Color outline_color, Color fill_color);
    void draw_segment(Vec2F a, Vec2F b, Color color);
    void draw_fat_segment(Vec2F a, Vec2F b, float radius, Color outline_color, Color fill_color);
    void draw_polygon(Span<const Vec2F> points, float radius, Color outline_color, Color fill_color);
    void draw_dot(float size, Vec2F location, Color color);

    void render(RenderRecorder& recorder) override;

  private:
    RenderManager* m_render_manager = nullptr;

    ShapeGroupData m_shape_group_data;
    ShapeGroup m_shape_group;
    CurveGroupData m_curve_group_data;
    CurveGroup m_curve_group;
  };

}

#endif // GF_DEBUG_ENTITY_H
