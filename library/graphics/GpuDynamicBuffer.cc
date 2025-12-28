// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/GpuDynamicBuffer.h>

namespace gf {

  GpuDynamicBuffer::GpuDynamicBuffer(Flags<GpuBufferUsage> usage)
  : m_usage(usage)
  {
  }

  void GpuDynamicBuffer::update(std::size_t member_size, std::size_t member_count, const void* data, RenderManager* render_manager)
  {
    m_current_buffer = (m_current_buffer + 1) % FramesInFlight;
    auto& current_buffer = m_buffers[m_current_buffer];

    if (member_count > current_buffer.member_count()) {
      current_buffer = GpuBuffer(m_usage, member_size, member_count, data, render_manager);

      if (!m_debug_name.empty()) {
        current_buffer.set_debug_name(m_debug_name + " #" + std::to_string(m_current_buffer));
      }
    } else {
      current_buffer.update(member_size, member_count, data, render_manager);
    }
  }

  void GpuDynamicBuffer::set_debug_name(const std::string& name)
  {
    m_debug_name = name;
  }

}
