// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/GpuBuffer.h>

#include <cstring>

#include <utility>

#include <gf2/core/Log.h>

#include <gf2/graphics/RenderManager.h>

namespace gf {

  GpuBuffer::GpuBuffer(Flags<GpuBufferUsage> usage, std::size_t size, const void* data, RenderManager* render_manager)
  : m_size(size)
  , m_usage(usage)
  {
    if (m_size == 0) {
      return;
    }

    SDL_GPUBufferCreateInfo info = {};
    info.usage = usage.value();
    info.size = size;

    GpuDevice* device = render_manager->device();

    SDL_GPUBuffer* buffer = SDL_CreateGPUBuffer(*device, &info);
    m_handle = { *device, buffer };

    update_device_buffer(size, data, render_manager);
  }

  void GpuBuffer::update(std::size_t size, const void* data, RenderManager* render_manager)
  {
    m_size = size;

    if (size == 0) {
      return;
    }

    update_device_buffer(size, data, render_manager);
  }

  void GpuBuffer::set_debug_name(const std::string& name)
  {
    SDL_SetGPUBufferName(m_handle.device(), m_handle.object(), name.c_str());
  }

  void GpuBuffer::update_device_buffer(std::size_t size, const void* data, RenderManager* render_manager)
  {
    Log::info("Size: {}", size);

    GpuTransferBuffer buffer(size, render_manager);
    buffer.update(size, data);

    GpuCopyPass copy_pass = render_manager->current_copy_pass();
    copy_pass.copy_buffer_to_buffer(&buffer, this, size);
    render_manager->defer_release_transfer_buffer(std::move(buffer));
  }

}
