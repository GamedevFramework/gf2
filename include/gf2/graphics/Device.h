// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_DEVICE_H
#define GF_DEVICE_H

#include <SDL3/SDL_gpu.h>

#include "GraphicsApi.h"

namespace gf {

  class GF_GRAPHICS_API Device {
  public:
    Device() = default;
    Device(const Device&) = delete;
    Device(Device&& other) noexcept;
    ~Device();

    Device& operator=(const Device&) = delete;
    Device& operator=(Device&& other) noexcept;

    explicit operator bool() const
    {
      return m_device != nullptr;
    }

  private:
    friend class RenderManager;

    Device(SDL_GPUDevice* device)
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


#endif // GF_DEVICE_H
