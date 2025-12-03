// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GPU_DEVICE_H
#define GF_GPU_DEVICE_H

#include <SDL3/SDL_gpu.h>

#include <gf2/core/Span.h>

#include "GraphicsApi.h"

namespace gf {

  class GF_GRAPHICS_API GpuDevice {
  public:
    GpuDevice() = default;
    GpuDevice(const GpuDevice&) = delete;
    GpuDevice(GpuDevice&& other) noexcept;
    ~GpuDevice();

    GpuDevice& operator=(const GpuDevice&) = delete;
    GpuDevice& operator=(GpuDevice&& other) noexcept;

    explicit operator bool() const
    {
      return m_device != nullptr;
    }

  private:
    friend class RenderManager;
    friend class Texture;
    friend class TransferBuffer;
    friend class GpuShader;

    GpuDevice(SDL_GPUDevice* device)
    : m_device(device)
    {
    }

    operator SDL_GPUDevice*()
    {
      return m_device;
    }

    SDL_GPUDevice* m_device = nullptr;
  };

}


#endif // GF_GPU_DEVICE_H
