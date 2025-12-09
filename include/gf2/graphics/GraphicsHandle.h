// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GRAPHICS_HANDLE_H
#define GF_GRAPHICS_HANDLE_H

#include <utility>

#include <SDL3/SDL_gpu.h>

namespace gf::details {

  template<typename T, void (*Destructor)(SDL_GPUDevice*, T*)>
  class GraphicsHandle {
  public:
    GraphicsHandle() = default;

    GraphicsHandle(SDL_GPUDevice* device, T* handle)
    : m_device(device)
    , m_handle(handle)
    {
    }

    GraphicsHandle(const GraphicsHandle&) = delete;

    GraphicsHandle(GraphicsHandle&& other) noexcept
    : m_device(other.m_device)
    , m_handle(std::exchange(other.m_handle, nullptr))
    {
    }

    ~GraphicsHandle()
    {
      if (m_handle != nullptr) {
        Destructor(m_device, m_handle);
      }
    }

    GraphicsHandle& operator=(const GraphicsHandle&) = delete;

    GraphicsHandle& operator=(GraphicsHandle&& other) noexcept
    {
      std::swap(other.m_device, m_device);
      std::swap(other.m_handle, m_handle);
      return *this;
    }

    T* object()
    {
      return m_handle;
    }

    const T* object() const
    {
      return m_handle;
    }

    operator T*()
    {
      return m_handle;
    }

    operator const T *() const
    {
      return m_handle;
    }

    explicit operator bool() const
    {
      return m_handle != nullptr;
    }

    SDL_GPUDevice* device() const
    {
      return m_device;
    }

  private:
    SDL_GPUDevice* m_device = nullptr;
    T* m_handle = nullptr;
  };

}


#endif // GF_GRAPHICS_HANDLE_H
