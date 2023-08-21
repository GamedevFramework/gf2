// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_BUFFER_H
#define GF_BUFFER_H

#include <cstdint>

#include <type_traits>

#include "GraphicsApi.h"
#include "Vulkan.h"

#include <vk_mem_alloc.h>

namespace gf {

  enum class BufferType : uint8_t {
    Device,
    Host,
  };

  // NOLINTNEXTLINE(performance-enum-size)
  enum class BufferUsage : std::underlying_type_t<VkBufferUsageFlagBits> {
    Upload = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    Storage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
    Uniform = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
    Index = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
    Vertex = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
  };

  class GF_GRAPHICS_API Buffer {
  public:
    Buffer() = default;
    Buffer(const Buffer&) = delete;
    Buffer(Buffer&& other) noexcept;
    ~Buffer();

    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer&& other) noexcept;

  private:
    friend class BasicRenderer;
    friend class CommandBuffer;

    Buffer(VmaAllocator allocator, VkBuffer buffer, VmaAllocation allocation, BufferType type, BufferUsage usage)
    : m_allocator(allocator)
    , m_buffer(buffer)
    , m_allocation(allocation)
    , m_type(type)
    , m_usage(usage)
    {
    }

    VmaAllocator m_allocator = nullptr; // non-owning
    VkBuffer m_buffer = VK_NULL_HANDLE;
    VmaAllocation m_allocation = nullptr;
    BufferType m_type = BufferType::Device;
    BufferUsage m_usage = BufferUsage::Vertex;
  };

}

#endif // GF_BUFFER_H
