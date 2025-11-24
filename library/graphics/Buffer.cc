// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Buffer.h>

#include <cstring>

#include <utility>

#include <gf2/core/Log.h>

#include <gf2/graphics/RenderManager.h>

namespace gf {

  Buffer::Buffer(BufferType type, BufferUsage usage, std::size_t size, std::size_t member_size, const void* data, RenderManager* render_manager)
  : m_size(size)
  , m_memory_size(size * member_size)
  , m_type(type)
  , m_usage(usage)
  {
    if (m_size == 0) {
      return;
    }

    switch (type) {
      case BufferType::Host:
        create_host_buffer(usage, m_memory_size, data);
        break;
      case BufferType::Device:
        create_device_buffer(usage, m_memory_size, data, render_manager);
        break;
    }
  }

  void Buffer::update(std::size_t size, std::size_t member_size, const void* data, RenderManager* render_manager)
  {
    const std::size_t total_size = size * member_size;
    assert(total_size <= m_memory_size);
    m_size = size;

    if (size == 0) {
      return;
    }

    switch (m_type) {
      case BufferType::Host:
        update_host_buffer(total_size, data);
        break;
      case BufferType::Device:
        update_device_buffer(total_size, data, render_manager);
        break;
    }
  }

  void Buffer::set_debug_name(const std::string& name) const
  {
  }

  void Buffer::create_host_buffer(BufferUsage usage, std::size_t total_size, const void* data)
  {
    // create buffer

    update_host_buffer(total_size, data);
  }

  void Buffer::create_device_buffer(BufferUsage usage, std::size_t total_size, const void* data, RenderManager* render_manager)
  {
    // create buffer

    update_device_buffer(total_size, data, render_manager);
  }

  void Buffer::update_host_buffer(std::size_t total_size, const void* data)
  {
  }

  void Buffer::update_device_buffer(std::size_t total_size, const void* data, RenderManager* render_manager)
  {
    // staging buffer


    // commands

    auto command_buffer = render_manager->current_copy_pass();


    // destroy staging buffer

  }

}
