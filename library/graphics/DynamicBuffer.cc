// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/DynamicBuffer.h>

namespace gf {

  DynamicBuffer::DynamicBuffer(Flags<GpuBufferUsage> usage)
  : m_usage(usage)
  {
  }

  void DynamicBuffer::update(std::size_t size, const void* data, RenderManager* render_manager)
  {
    m_current_buffer = (m_current_buffer + 1) % FramesInFlight;
    auto& current_buffer = m_buffers[m_current_buffer]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

    if (current_buffer.empty()) {
      current_buffer = GpuBuffer(m_usage, size, data, render_manager);

      if (!m_debug_name.empty()) {
        current_buffer.set_debug_name(m_debug_name + " #" + std::to_string(m_current_buffer));
      }
    } else {
      current_buffer.update(size, data, render_manager);
    }
  }

  void DynamicBuffer::set_debug_name(const std::string& name)
  {
    m_debug_name = name;
  }

}
