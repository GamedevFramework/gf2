// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/DebugEntity.h>
// clang-format on

#include <gf2/core/Mat3.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  namespace {

    constexpr int DebugPointCount = 20;
    constexpr float DebugLineThickness = 1.0f;
    constexpr float DebugAlpha = 0.8f;

  }

  DebugEntity::DebugEntity(RenderManager* render_manager)
  : m_render_manager(render_manager)
  {
  }

  void DebugEntity::start_debug()
  {
    m_shape_group_data.shapes.clear();
    m_curve_group_data.curves.clear();
  }

  void DebugEntity::draw_circle(Vec2F center, float angle, float radius, Color outline_color, Color fill_color)
  {
    fill_color.a = DebugAlpha;

    auto shape = ShapeData::make_circle(CircF::from_center_radius(center, radius), DebugPointCount);
    shape.color = fill_color;
    shape.outline_thickness = DebugLineThickness;
    shape.outline_color = outline_color;
    m_shape_group_data.shapes.push_back(std::move(shape));

    auto curve = CurveData::make_line(center, center + radius * unit(angle));
    curve.color = outline_color;
    m_curve_group_data.curves.push_back(std::move(curve));
  }

  void DebugEntity::draw_segment(Vec2F a, Vec2F b, Color color)
  {
    auto curve = CurveData::make_line(a, b);
    curve.color = color;
    m_curve_group_data.curves.push_back(std::move(curve));
  }

  void DebugEntity::draw_fat_segment(Vec2F a, Vec2F b, float radius, Color outline_color, Color fill_color)
  {
    fill_color.a = DebugAlpha;

    auto curve = CurveData::make_line(a, b);
    curve.thickness = 2.0f * radius;
    curve.color = fill_color;
    curve.outline_thickness = DebugLineThickness;
    curve.outline_color = outline_color;
    m_curve_group_data.curves.push_back(std::move(curve));
  }

  void DebugEntity::draw_polygon(Span<const Vec2F> points, float radius, Color outline_color, Color fill_color)
  {
    fill_color.a = DebugAlpha;

    auto shape = ShapeData::make_polygon(points);
    shape.color = fill_color;
    shape.outline_thickness = radius;
    shape.outline_color = outline_color;
    m_shape_group_data.shapes.push_back(std::move(shape));
  }

  void DebugEntity::draw_dot(float size, Vec2F location, Color color)
  {
    color.a = DebugAlpha;

    auto shape = ShapeData::make_circle(CircF::from_center_radius(location, size), DebugPointCount);
    shape.color = color;
    m_shape_group_data.shapes.push_back(std::move(shape));
  }

  void DebugEntity::stop_debug()
  {
    m_shape_group.update(m_shape_group_data, m_render_manager);
    m_curve_group.update(m_curve_group_data, m_render_manager);
  }

  void DebugEntity::render(RenderRecorder& recorder)
  {
    RenderObject object = {};
    object.priority = priority();
    object.transform = Identity3F;

    object.geometry = m_shape_group.geometry();

    if (object.geometry.size > 0) {
      recorder.record(object);
    }

    object.geometry = m_curve_group.geometry();

    if (object.geometry.size > 0) {
      recorder.record(object);
    }
  }

}
