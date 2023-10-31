// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/RenderRecorder.h>
// clang-format on

#include <cassert>

namespace gf {

  RenderRecorder::RenderRecorder(RenderManager* render_manager)
  : m_render_manager(render_manager)
  {
  }

  void RenderRecorder::update_view(const Mat4F& view_matrix, const RectF& viewport)
  {
    if (m_next_view_matrix_index == m_view_matrix_buffers.size()) {
      Buffer buffer(BufferType::Device, BufferUsage::Uniform, &view_matrix, 1, m_render_manager);
      buffer.set_debug_name("[gf2] View Matrix Buffer #" + std::to_string(m_next_view_matrix_index));
      m_view_matrix_buffers.push_back(std::move(buffer));
    } else {
      m_view_matrix_buffers[m_next_view_matrix_index].update(&view_matrix, 1, m_render_manager);
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
    m_objects.clear();
    m_next_view_matrix_index = 0;
  }

}
