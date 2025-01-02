// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_DYNAMIC_BUFFER
#define GF_DYNAMIC_BUFFER

#include <string>

#include "Buffer.h"
#include "GraphicsApi.h"

namespace gf {

  class GF_GRAPHICS_API DynamicBuffer {
  public:
    DynamicBuffer(BufferType type, BufferUsage usage);

    Buffer& buffer()
    {
      return m_buffers[m_current_buffer]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    const Buffer& buffer() const
    {
      return m_buffers[m_current_buffer]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    void update(std::size_t size, std::size_t member_size, const void* data, RenderManager* render_manager);

    template<typename T>
    void update(const T* data, std::size_t size, RenderManager* render_manager)
    {
      update(size, sizeof(T), static_cast<const void*>(data), render_manager);
    }

    void set_debug_name(const std::string& name);

  private:
    static constexpr std::size_t FramesInFlight = 3;

    BufferType m_type;
    BufferUsage m_usage;
    std::size_t m_current_buffer = 0;
    Buffer m_buffers[FramesInFlight];
    std::string m_debug_name;
  };

}

#endif // GF_DYNAMIC_BUFFER
