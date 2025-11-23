// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Device.h>

#include <utility>

namespace gf {

  Device::Device(Device&& other) noexcept
  : m_device(std::exchange(other.m_device, nullptr))
  {
  }

  Device::~Device()
  {
    if (m_device != nullptr) {
      SDL_DestroyGPUDevice(m_device);
    }
  }

  Device& Device::operator=(Device&& other) noexcept
  {
    std::swap(other.m_device, m_device);
    return *this;
  }

}
