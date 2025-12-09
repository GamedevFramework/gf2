// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/GpuTransferBuffer.h>

#include <cassert>
#include <cstring>

#include <gf2/core/Log.h>

#include <gf2/graphics/RenderManager.h>

namespace gf {

  GpuTransferBuffer::GpuTransferBuffer(std::size_t size, RenderManager* render_manager)
  {
    SDL_GPUTransferBufferCreateInfo info = { };
    info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    info.size = static_cast<Uint32>(size);

    GpuDevice* device = render_manager->device();
    assert(device);

    SDL_GPUTransferBuffer* buffer = SDL_CreateGPUTransferBuffer(*device, &info);

    if (buffer == nullptr) {
      Log::fatal("Could not create transfer buffer: {}", SDL_GetError());
    }

    m_handle = { *device, buffer };
  }

  void GpuTransferBuffer::update(std::size_t size, const void* data)
  {
    assert(m_handle);
    void* destination = SDL_MapGPUTransferBuffer(m_handle.device(), m_handle, false);

    if (destination == nullptr) {
      Log::fatal("Could not map transfer buffer: {}", SDL_GetError());
    }

    std::memcpy(destination, data, size);
    SDL_UnmapGPUTransferBuffer(m_handle.device(), m_handle);
  }

}
