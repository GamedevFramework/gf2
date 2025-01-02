// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/framework/GraphicsPhysicsDebug.h>

#include <gf2/core/Mat3.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  namespace {

    constexpr int DebugPointCount = 20;
    constexpr float DebugLineThickness = 1.0f;
    constexpr float DebugAlpha = 0.8f;

  }

  /*
   * GraphicsPhysicsDebug
   */

  void GraphicsPhysicsDebug::clear()
  {
    m_shape_group_buffer.shapes.clear();
    m_curve_group_buffer.curves.clear();
  }

  void GraphicsPhysicsDebug::draw_circle(Vec2F center, float angle, float radius, Color outline_color, Color fill_color)
  {
    fill_color.a = DebugAlpha;

    auto shape = ShapeBuffer::make_circle(CircF::from_center_radius(center, radius), DebugPointCount);
    shape.color = fill_color;
    shape.outline_thickness = DebugLineThickness;
    shape.outline_color = outline_color;
    m_shape_group_buffer.shapes.push_back(std::move(shape));

    auto curve = CurveBuffer::make_line(center, center + radius * unit(angle));
    curve.color = outline_color;
    m_curve_group_buffer.curves.push_back(std::move(curve));
  }

  void GraphicsPhysicsDebug::draw_segment(Vec2F a, Vec2F b, Color color)
  {
    auto curve = CurveBuffer::make_line(a, b);
    curve.color = color;
    m_curve_group_buffer.curves.push_back(std::move(curve));
  }

  void GraphicsPhysicsDebug::draw_fat_segment(Vec2F a, Vec2F b, float radius, Color outline_color, Color fill_color)
  {
    fill_color.a = DebugAlpha;

    auto curve = CurveBuffer::make_line(a, b);
    curve.thickness = 2.0f * radius;
    curve.color = fill_color;
    curve.outline_thickness = DebugLineThickness;
    curve.outline_color = outline_color;
    m_curve_group_buffer.curves.push_back(std::move(curve));
  }

  void GraphicsPhysicsDebug::draw_polygon(Span<const Vec2F> points, float radius, Color outline_color, Color fill_color)
  {
    fill_color.a = DebugAlpha;

    auto shape = ShapeBuffer::make_polygon(points);
    shape.color = fill_color;
    shape.outline_thickness = radius;
    shape.outline_color = outline_color;
    m_shape_group_buffer.shapes.push_back(std::move(shape));
  }

  void GraphicsPhysicsDebug::draw_dot(float size, Vec2F location, Color color)
  {
    color.a = DebugAlpha;

    auto shape = ShapeBuffer::make_circle(CircF::from_center_radius(location, size), DebugPointCount);
    shape.color = color;
    m_shape_group_buffer.shapes.push_back(std::move(shape));
  }

  /*
   * GraphicsPhysicsDebugEntity
   */

  GraphicsPhysicsDebugEntity::GraphicsPhysicsDebugEntity(PhysicsWorld* physics_world, RenderManager* render_manager)
  : m_physics_world(physics_world)
  , m_render_manager(render_manager)
  {
  }

  void GraphicsPhysicsDebugEntity::update([[maybe_unused]] Time time)
  {
    if (m_awake) {
      m_debug.clear();

      m_physics_world->debug_draw(&m_debug);

      m_shape_group.update(m_debug.shapes(), m_render_manager);
      m_curve_group.update(m_debug.curves(), m_render_manager);
    }
  }

  void GraphicsPhysicsDebugEntity::render(RenderRecorder& recorder)
  {
    if (!m_awake) {
      return;
    }

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
