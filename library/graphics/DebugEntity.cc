// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <cstdint>
#include <gf2/graphics/DebugEntity.h>
// clang-format on

#include <gf2/core/Mat3.h>

#include <gf2/graphics/RenderRecorder.h>
#include "gf2/graphics/RenderObject.h"

namespace gf {

  namespace {

    constexpr int DebugPointCount = 20;
    constexpr float DebugLineThickness = 1.0f;

  }

  DebugEntity::DebugEntity(RenderManager* render_manager)
  : m_render_manager(render_manager)
  {
  }

  void DebugEntity::start_debug()
  {
    m_raw_vertices.clear();
    m_raw_indices.clear();
  }

  void DebugEntity::draw_circle(Vec2F center, float angle, float radius, Color outline_color, Color fill_color)
  {
    fill_color.a = 0.8f;
    draw_dot(radius, center, fill_color);

    // outline

    auto base_index = static_cast<uint16_t>(m_raw_vertices.size());

    for (int i = 0; i < DebugPointCount; ++i) {
      const float angle = static_cast<float>(i) * 2.0f * Pi / static_cast<float>(DebugPointCount);
      m_raw_vertices.push_back({ center + radius * unit(angle), { 0.0f, 0.0f }, outline_color });
      m_raw_vertices.push_back({ center + (radius + DebugLineThickness) * unit(angle), { 0.0f, 0.0f }, outline_color });
    }

    // simulate triangle strip
    for (int i = 0; i < 2 * DebugPointCount; ++i) {
      m_raw_indices.push_back(base_index + i + 0);
      m_raw_indices.push_back(base_index + (i + 1) % (2 * DebugPointCount));
      m_raw_indices.push_back(base_index + (i + 2) % (2 * DebugPointCount));
    }

    draw_segment(center, center + radius * unit(angle), outline_color);
  }

  void DebugEntity::draw_segment(Vec2F a, Vec2F b, Color color)
  {
    auto base_index = static_cast<uint16_t>(m_raw_vertices.size());

    auto normal = normalize(perp(a - b));
    m_raw_vertices.push_back({ a + 0.5f * DebugLineThickness * normal, { 0.0f, 0.0f }, color });
    m_raw_vertices.push_back({ b + 0.5f * DebugLineThickness * normal, { 0.0f, 0.0f }, color });
    m_raw_vertices.push_back({ b - 0.5f * DebugLineThickness * normal, { 0.0f, 0.0f }, color });
    m_raw_vertices.push_back({ a - 0.5f * DebugLineThickness * normal, { 0.0f, 0.0f }, color });

    m_raw_indices.push_back(base_index);
    m_raw_indices.push_back(base_index + 1);
    m_raw_indices.push_back(base_index + 2);

    m_raw_indices.push_back(base_index + 2);
    m_raw_indices.push_back(base_index + 1);
    m_raw_indices.push_back(base_index + 3);
  }

  void DebugEntity::draw_fat_segment(Vec2F a, Vec2F b, float radius, Color outline_color, Color fill_color)
  {
    fill_color.a = 0.8f;

    auto base_index = static_cast<uint16_t>(m_raw_vertices.size());

    auto normal = normalize(perp(a - b));
    m_raw_vertices.push_back({ a + 0.5f * radius * normal, { 0.0f, 0.0f }, fill_color });
    m_raw_vertices.push_back({ b + 0.5f * radius * normal, { 0.0f, 0.0f }, fill_color });
    m_raw_vertices.push_back({ b - 0.5f * radius * normal, { 0.0f, 0.0f }, fill_color });
    m_raw_vertices.push_back({ a - 0.5f * radius * normal, { 0.0f, 0.0f }, fill_color });

    m_raw_indices.push_back(base_index);
    m_raw_indices.push_back(base_index + 1);
    m_raw_indices.push_back(base_index + 2);

    m_raw_indices.push_back(base_index + 2);
    m_raw_indices.push_back(base_index + 1);
    m_raw_indices.push_back(base_index + 3);

    // TODO: outline
  }

  void DebugEntity::draw_polygon(Span<const Vec2F> points, float radius, Color outline_color, Color fill_color)
  {
    fill_color.a = 0.8f;

    Vec2F center = { 0.0f, 0.0f };

    for (auto point : points) {
      center += point;
    }

    center /= points.size();

    auto center_index = static_cast<uint16_t>(m_raw_vertices.size());
    m_raw_vertices.push_back({ center, { 0.0f, 0.0f }, fill_color });

    for (auto point : points) {
      m_raw_vertices.push_back({ point, { 0.0f, 0.0f }, fill_color });
    }

    // simulate triangle fan
    for (std::size_t i = 0; i < points.size(); ++i) {
      m_raw_indices.push_back(center_index);
      m_raw_indices.push_back(center_index + static_cast<uint16_t>(1 + i));
      m_raw_indices.push_back(center_index + static_cast<uint16_t>(1 + (i + 1) % points.size()));
    }

    // TODO: outline
  }

  void DebugEntity::draw_dot(float size, Vec2F location, Color color)
  {
    auto center_index = static_cast<uint16_t>(m_raw_vertices.size());
    m_raw_vertices.push_back({ location, { 0.0f, 0.0f }, color });

    for (int i = 0; i < DebugPointCount; ++i) {
      const float angle = static_cast<float>(i) * 2.0f * Pi / static_cast<float>(DebugPointCount);
      m_raw_vertices.push_back({ location + size * unit(angle), { 0.0f, 0.0f }, color });
    }

    // simulate triangle fan
    for (int i = 0; i < DebugPointCount; ++i) {
      m_raw_indices.push_back(center_index);
      m_raw_indices.push_back(center_index + static_cast<uint16_t>(1 + i));
      m_raw_indices.push_back(center_index + static_cast<uint16_t>(1 + (i + 1) % DebugPointCount));
    }
  }

  void DebugEntity::stop_debug()
  {
    m_current_buffer = (m_current_buffer + 1) % DebugFramesInFlight;
    auto& current_vertices = m_vertices[m_current_buffer]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    auto& current_indices = m_indices[m_current_buffer]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

    if (m_raw_vertices.size() > current_vertices.size()) {
      current_vertices = Buffer(BufferType::Host, BufferUsage::Vertex, m_raw_vertices.data(), m_raw_vertices.size(), m_render_manager);
      current_vertices.set_debug_name("[gf2] Physics Debug Vertex Buffer #" + std::to_string(m_current_buffer));
    } else {
      current_vertices.update(m_raw_vertices.data(), m_raw_vertices.size(), m_render_manager);
    }

    if (m_raw_indices.size() > current_indices.size()) {
      current_indices = Buffer(BufferType::Host, BufferUsage::Index, m_raw_indices.data(), m_raw_indices.size(), m_render_manager);
      current_indices.set_debug_name("[gf2] Physics Debug Index Buffer #" + std::to_string(m_current_buffer));
    } else {
      current_indices.update(m_raw_indices.data(), m_raw_indices.size(), m_render_manager);
    }
  }

  void DebugEntity::render(RenderRecorder& recorder)
  {
    RenderObject object = {};
    object.priority = priority();
    object.geometry.pipeline = RenderPipelineType::Default;
    object.geometry.vertices = &m_vertices[m_current_buffer]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    object.geometry.indices = &m_indices[m_current_buffer]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    object.geometry.count = m_indices[m_current_buffer].count(); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    object.transform = Identity3F;
    recorder.record(object);
  }

}
