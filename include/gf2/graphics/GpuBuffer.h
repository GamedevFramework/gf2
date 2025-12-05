// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_BUFFER_H
#define GF_BUFFER_H

#include <string>

#include <SDL3/SDL_gpu.h>

#include <gf2/core/Flags.h>

#include "GraphicsApi.h"
#include "GraphicsHandle.h"

namespace gf {
  class RenderManager;

  // NOLINTNEXTLINE(performance-enum-size)
  enum class GpuBufferUsage : SDL_GPUBufferUsageFlags {
    Vertex = SDL_GPU_BUFFERUSAGE_VERTEX,
    Index = SDL_GPU_BUFFERUSAGE_INDEX,
    Indirect = SDL_GPU_BUFFERUSAGE_INDIRECT,
    GraphicsStorageRead = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
    ComputeStorageRead = SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ,
    ComputeStorageWrite = SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE,
  };

  template<>
  struct EnableBitmaskOperators<GpuBufferUsage> : std::true_type {
  };

  class GF_GRAPHICS_API GpuBuffer {
  public:
    GpuBuffer() = default;

    GpuBuffer(Flags<GpuBufferUsage> usage, std::size_t size, const void* data, RenderManager* render_manager);

    template<typename T>
    GpuBuffer(Flags<GpuBufferUsage> usage, const T* data, std::size_t size, RenderManager* render_manager)
    : GpuBuffer(usage, size * sizeof(T), static_cast<const void*>(data), render_manager)
    {
    }

    std::size_t size() const
    {
      return m_size;
    }

    bool empty() const
    {
      return m_size == 0;
    }

    void update(std::size_t size, const void* data, RenderManager* render_manager);

    template<typename T>
    void update(const T* data, std::size_t size, RenderManager* render_manager)
    {
      update(size * sizeof(T), static_cast<const void*>(data), render_manager);
    }

    void set_debug_name(const std::string& name);

  private:
    friend class GpuRenderPass;
    friend class Descriptor;

    void update_device_buffer(std::size_t size, const void* data, RenderManager* render_manager);

    details::GraphicsHandle<SDL_GPUBuffer, SDL_ReleaseGPUBuffer> m_handle;

    std::size_t m_size = 0;
    Flags<GpuBufferUsage> m_usage = GpuBufferUsage::Vertex;
  };

}

#endif // GF_BUFFER_H
