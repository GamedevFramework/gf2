// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/RenderRecorder.h>
// clang-format on

#include <cassert>

#include <gf2/graphics/Buffer.h>

namespace gf {

  RenderRecorder::RenderRecorder(Renderer* renderer)
  : m_renderer(renderer)
  {
  }

  void RenderRecorder::update_view(const Mat4F& view_matrix, const RectF& viewport)
  {
    if (m_next_view_matrix_index == m_view_matrix_buffers.size()) {
      Buffer buffer(BufferType::Device, BufferUsage::Uniform, &view_matrix, 1, m_renderer);
      m_view_matrix_buffers.push_back(std::move(buffer));
    } else {
      m_view_matrix_buffers[m_next_view_matrix_index].update(&view_matrix, 1, m_renderer);
    }

    std::vector<RenderObject> objects;
    m_parts.push_back({ m_next_view_matrix_index, viewport , objects });
    ++m_next_view_matrix_index;
  }

  void RenderRecorder::record(const RenderObject& object)
  {
    if (m_parts.empty()) {
      // TODO: warning
      return;
    }

    m_parts.back().objects.push_back(object);
  }

  void RenderRecorder::sort()
  {
    // TODO
  }

  void RenderRecorder::clear()
  {
    m_next_view_matrix_index = 0;
    m_parts.clear();
  }

}
