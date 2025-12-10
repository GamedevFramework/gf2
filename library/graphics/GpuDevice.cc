// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/GpuDevice.h>

#include <utility>

namespace gf {

  GpuDevice::GpuDevice(GpuDevice&& other) noexcept
  : m_device(std::exchange(other.m_device, nullptr))
  {
  }

  GpuDevice::~GpuDevice()
  {
    if (m_device != nullptr) {
      SDL_WaitForGPUIdle(m_device);
      SDL_DestroyGPUDevice(m_device);
    }
  }

  GpuDevice& GpuDevice::operator=(GpuDevice&& other) noexcept
  {
    std::swap(other.m_device, m_device);
    return *this;
  }

}
