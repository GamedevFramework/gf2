// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/RenderRecorder.h>

#include <cassert>

#include <array>

namespace gf {

  namespace {

    std::array<float, 5> compute_raw(const TextEffect& effect)
    {
      return { effect.outline_color.r, effect.outline_color.g, effect.outline_color.b, effect.outline_color.a, effect.outline_thickness };
    }

  }

  RenderRecorder::RenderRecorder(RenderManager* render_manager)
  : m_render_manager(render_manager)
  {
  }

  void RenderRecorder::update_view(const Mat3F& view_matrix, const RectF& viewport)
  {
    auto aligned_view_matrix = compute_aligned(view_matrix);

    if (m_next_view_matrix_index == m_view_matrix_buffers.size()) {
      Buffer buffer(BufferType::Device, BufferUsage::Uniform, aligned_view_matrix.size(), sizeof(float), aligned_view_matrix.data(), m_render_manager);
      buffer.set_debug_name("[gf2] View Matrix Buffer #" + std::to_string(m_next_view_matrix_index));
      m_view_matrix_buffers.push_back(std::move(buffer));
    } else {
      m_view_matrix_buffers[m_next_view_matrix_index].update(aligned_view_matrix.size(), sizeof(float), aligned_view_matrix.data(), m_render_manager);
    }

    const Record record = { RecordType::View, m_views.size() };
    m_records.push_back(record);

    m_views.push_back({ m_next_view_matrix_index, viewport });
    ++m_next_view_matrix_index;
  }

  void RenderRecorder::update_scissor(const RectI& scissor)
  {
    const Record record = { RecordType::Scissor, m_scissors.size() };
    m_records.push_back(record);

    m_scissors.push_back({ scissor });
  }

  void RenderRecorder::update_text_effect(const TextEffect& effect)
  {
    auto effect_data = compute_raw(effect);

    if (m_next_text_effect_index == m_text_effect_buffers.size()) {
      Buffer buffer(BufferType::Device, BufferUsage::Uniform, effect_data.size(), sizeof(float), effect_data.data(), m_render_manager);
      buffer.set_debug_name("[gf2] Text Effect Buffer #" + std::to_string(m_next_text_effect_index));
      m_text_effect_buffers.push_back(std::move(buffer));
    } else {
      m_text_effect_buffers[m_next_text_effect_index].update(effect_data.size(), sizeof(float), effect_data.data(), m_render_manager);
    }

    const Record record = { RecordType::Text, m_texts.size() };
    m_records.push_back(record);

    m_texts.push_back({ m_next_text_effect_index });
    ++m_next_text_effect_index;
  }

  void RenderRecorder::record(const RenderObject& object)
  {
    const Record record = { RecordType::Object, m_objects.size() };
    m_records.push_back(record);

    m_objects.push_back(object);
  }

  void RenderRecorder::sort()
  {
    // TODO
  }

  void RenderRecorder::clear()
  {
    m_records.clear();
    m_views.clear();
    m_scissors.clear();
    m_texts.clear();
    m_objects.clear();
    m_next_view_matrix_index = 0;
    m_next_text_effect_index = 0;
  }

}
