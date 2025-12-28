// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/framework/GraphicsPhysicsDebug.h>

#include <limits>

#include <gf2/core/Mat3.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  namespace {

    constexpr int DebugPointCount = 20;
    constexpr float DebugLineThickness = 1.0f;
    constexpr float DebugTransformLength = 64.0f;

  }

  /*
   * GraphicsPhysicsDebug
   */

  void GraphicsPhysicsDebug::clear()
  {
    m_shape_group_buffer.shapes.clear();
    m_curve_group_buffer.curves.clear();
  }

  void GraphicsPhysicsDebug::draw_polygon(Span<const Vec2F> polygon, Color color)
  {
    CurveBuffer curve = CurveBuffer::make_compound_curve(polygon, PolylineType::Loop);
    curve.color = color;
    curve.thickness = DebugLineThickness;
    m_curve_group_buffer.curves.push_back(std::move(curve));
  }

  void GraphicsPhysicsDebug::draw_solid_polygon(Span<const Vec2F> polygon, float radius, Color color)
  {
    ShapeBuffer shape = ShapeBuffer::make_polygon(polygon);
    shape.color = color;
    shape.outline_thickness = radius;
    shape.outline_color = darker(color);
    m_shape_group_buffer.shapes.push_back(std::move(shape));
  }

  void GraphicsPhysicsDebug::draw_circle(CircF circle, Color color)
  {
    CurveBuffer curve = CurveBuffer::make_circle(circle, DebugPointCount);
    curve.color = color;
    curve.thickness = DebugLineThickness;
    m_curve_group_buffer.curves.push_back(std::move(curve));
  }

  void GraphicsPhysicsDebug::draw_solid_circle(CircF circle, float angle, Color color)
  {
    ShapeBuffer shape = ShapeBuffer::make_circle(circle, DebugPointCount);
    shape.color = color;
    m_shape_group_buffer.shapes.push_back(std::move(shape));

    CurveBuffer curve = CurveBuffer::make_line(circle.center, circle.center + circle.radius * unit(angle));
    curve.color = darker(color);
    curve.thickness = DebugLineThickness;
    m_curve_group_buffer.curves.push_back(std::move(curve));
  }

  void GraphicsPhysicsDebug::draw_solid_capsule(const CapsuleF& capsule, Color color)
  {
    ShapeBuffer shape = ShapeBuffer::make_capsule(capsule, DebugPointCount);
    shape.color = color;
    m_shape_group_buffer.shapes.push_back(std::move(shape));
  }

  void GraphicsPhysicsDebug::draw_line(const SegmentF& segment, Color color)
  {
    CurveBuffer curve = CurveBuffer::make_line(segment);
    curve.color = color;
    curve.thickness = DebugLineThickness;
    m_curve_group_buffer.curves.push_back(std::move(curve));
  }

  void GraphicsPhysicsDebug::draw_transform(const PhysicsTransform& transform)
  {
    const float angle = transform.rotation.angle();
    const Vec2F origin = transform.location;
    const Vec2F unit_x = origin + gf::unit(angle) * DebugTransformLength;
    const Vec2F unit_y = origin + gf::unit(angle + Pi2) * DebugTransformLength;

    CurveBuffer curve_x = CurveBuffer::make_line(origin, unit_x);
    curve_x.color = Red;
    curve_x.thickness = DebugLineThickness;
    m_curve_group_buffer.curves.push_back(std::move(curve_x));

    CurveBuffer curve_y = CurveBuffer::make_line(origin, unit_y);
    curve_y.color = Green;
    curve_y.thickness = DebugLineThickness;
    m_curve_group_buffer.curves.push_back(std::move(curve_y));
  }

  void GraphicsPhysicsDebug::draw_point(CircF point, Color color)
  {
    ShapeBuffer shape = ShapeBuffer::make_circle(point, DebugPointCount);
    shape.color = color;
    m_shape_group_buffer.shapes.push_back(std::move(shape));
  }

  void GraphicsPhysicsDebug::draw_string(Vec2F point, std::string_view string, Color color)
  {
    TextDebugData text;
    text.content = string;
    text.color = color;
    text.location = point;
    m_texts.push_back(std::move(text));
  }

  /*
   * GraphicsPhysicsDebugEntity
   */

  GraphicsPhysicsDebugEntity::GraphicsPhysicsDebugEntity(const PhysicsDebugOptions& options, PhysicsWorld* physics_world, RenderManager* render_manager)
  : m_options(options)
  , m_physics_world(physics_world)
  , m_render_manager(render_manager)
  {
    set_priority(std::numeric_limits<int32_t>::max());
  }

  void GraphicsPhysicsDebugEntity::update([[maybe_unused]] Time time)
  {
    if (!m_awake) {
      return;
    }

    m_debug.clear();

    m_physics_world->debug_draw(&m_debug, m_options);

    m_shape_group.update(m_debug.shapes(), m_render_manager);
    m_curve_group.update(m_debug.curves(), m_render_manager);

    if (m_atlas == nullptr || m_font_face == nullptr || m_character_size <= 0.0f) {
      return;
    }

    const std::vector<TextDebugData>& debug_texts = m_debug.texts();
    const std::size_t text_count = debug_texts.size();

    for (std::size_t i = 0; i < text_count; ++i) {
      const TextDebugData& debug_text = debug_texts[i];

      TextData data;
      data.content = debug_text.content;
      data.color = debug_text.color;
      data.character_size = m_character_size;

      if (i < m_texts.size()) {
        TextEntity& entity = m_texts[i];
        entity.set_location(debug_text.location);
        entity.text().update(data, m_render_manager);
      } else {
        m_texts.emplace_back(m_atlas, m_font_face, data, m_render_manager);
      }
    }

    for (std::size_t i = text_count; i < m_texts.size(); ++i) {
      TextData data;
      TextEntity& entity = m_texts[i];
      entity.text().update(data, m_render_manager);
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

    if (object.geometry.count > 0) {
      recorder.record(object);
    }

    object.geometry = m_curve_group.geometry();

    if (object.geometry.count > 0) {
      recorder.record(object);
    }

    for (TextEntity& text : m_texts) {
      text.render(recorder);
    }
  }

}
