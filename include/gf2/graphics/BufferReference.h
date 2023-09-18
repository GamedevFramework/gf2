// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_BUFFER_REFERENCE_H
#define GF_BUFFER_REFERENCE_H

#include <vk_mem_alloc.h>

#include "GraphicsApi.h"
#include "Vulkan.h"

namespace gf {

  class BufferReference {
  public:

  private:
    friend class MemoryCommandBuffer;
    friend class Buffer;
    friend class Texture;

    BufferReference(VkBuffer buffer)
    : m_buffer(buffer)
    {
    }

    VkBuffer m_buffer = VK_NULL_HANDLE;
  };

  class StagingBufferReference {
  public:

  private:
    friend class BasicRenderer;
    friend class Buffer;
    friend class Texture;

    StagingBufferReference(VkBuffer buffer, VmaAllocation allocation)
    : m_buffer(buffer)
    , m_allocation(allocation)
    {
    }

    VkBuffer m_buffer = VK_NULL_HANDLE;
    VmaAllocation m_allocation = nullptr;
  };

}

#endif // GF_BUFFER_REFERENCE_H
