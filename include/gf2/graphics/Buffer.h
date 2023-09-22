// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_BUFFER_H
#define GF_BUFFER_H

#include <cstdint>

#include <string>
#include <type_traits>

#include <vk_mem_alloc.h>

#include "GraphicsApi.h"
#include "Vulkan.h"

namespace gf {
  class Renderer;

  enum class BufferType : uint8_t {
    Device,
    Host,
  };

  // NOLINTNEXTLINE(performance-enum-size)
  enum class BufferUsage : std::underlying_type_t<VkBufferUsageFlagBits> {
    Storage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
    Uniform = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
    Index = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
    Vertex = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
  };

  class GF_GRAPHICS_API Buffer {
  public:
    Buffer() = default;

    Buffer(BufferType type, BufferUsage usage, std::size_t size, std::size_t member_size, const void* data, Renderer* renderer);

    template<typename T>
    Buffer(BufferType type, BufferUsage usage, const T* data, std::size_t size, Renderer* renderer)
    : Buffer(type, usage, size, sizeof(T), static_cast<const void*>(data), renderer)
    {
    }

    Buffer(const Buffer&) = delete;
    Buffer(Buffer&& other) noexcept;
    ~Buffer();

    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer&& other) noexcept;

    void update(std::size_t size, std::size_t member_size, const void* data, Renderer* renderer);

    template<typename T>
    void update(const T* data, std::size_t size, Renderer* renderer)
    {
      update(size, sizeof(T), static_cast<const void*>(data), renderer);
    }

    void set_debug_name(const std::string& name) const;

  private:
    friend class RenderCommandBuffer;
    friend class Descriptor;

    void create_host_buffer(BufferUsage usage, std::size_t total_size, const void* data);
    void create_device_buffer(BufferUsage usage, std::size_t total_size, const void* data, Renderer* renderer);

    void update_host_buffer(std::size_t total_size, const void* data);
    void update_device_buffer(std::size_t total_size, const void* data, Renderer* renderer);

    VmaAllocator m_allocator = nullptr; // non-owning
    VkBuffer m_buffer = VK_NULL_HANDLE;
    VmaAllocation m_allocation = nullptr;
    std::size_t m_size = 0;
    std::size_t m_member_size = 0;
    BufferType m_type = BufferType::Device;
    BufferUsage m_usage = BufferUsage::Vertex;
  };

}

#endif // GF_BUFFER_H
