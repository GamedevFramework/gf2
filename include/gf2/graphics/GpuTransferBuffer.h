// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GPU_TRANSFER_BUFFER_H
#define GF_GPU_TRANSFER_BUFFER_H

#include <type_traits>

#include <SDL3/SDL_gpu.h>

#include "GraphicsApi.h"
#include "GraphicsHandle.h"

namespace gf {
  class RenderManager;

  // NOLINTNEXTLINE(performance-enum-size)
  enum class GpuTransferBufferUsage : std::underlying_type_t<SDL_GPUTransferBufferUsage> {
    Upload = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
    Download = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD,
  };

  class GF_GRAPHICS_API GpuTransferBuffer {
  public:
    GpuTransferBuffer() = default;
    GpuTransferBuffer(std::size_t size, RenderManager* render_manager);

    void update(std::size_t size, const void* data);

  private:
    GpuTransferBuffer(SDL_GPUDevice* device, SDL_GPUTransferBuffer* buffer)
    : m_handle(device, buffer)
    {
    }

    details::GraphicsHandle<SDL_GPUTransferBuffer, SDL_ReleaseGPUTransferBuffer> m_handle;
  };

}

#endif // GF_GPU_TRANSFER_BUFFER_H
