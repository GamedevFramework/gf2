// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Buffer.h>
// clang-format on

#include <cstring>

#include <stdexcept>
#include <utility>

#include <volk.h>

#include <gf2/core/Log.h>

#include <gf2/graphics/Renderer.h>

namespace gf {

  Buffer::Buffer(BufferType type, BufferUsage usage, std::size_t count, std::size_t member_size, const void* data, Renderer* renderer)
  : m_allocator(renderer->m_allocator)
  , m_count(count)
  , m_size(count * member_size)
  , m_type(type)
  , m_usage(usage)
  {
    switch (type) {
      case BufferType::Host:
        create_host_buffer(usage, m_size, data);
        break;
      case BufferType::Device:
        create_device_buffer(usage, m_size, data, renderer);
        break;
    }
  }

  Buffer::Buffer(Buffer&& other) noexcept
  : m_allocator(std::exchange(other.m_allocator, nullptr))
  , m_buffer(std::exchange(other.m_buffer, VK_NULL_HANDLE))
  , m_allocation(std::exchange(other.m_allocation, nullptr))
  , m_count(other.m_count)
  , m_size(other.m_size)
  , m_type(other.m_type)
  , m_usage(other.m_usage)
  {
  }

  Buffer::~Buffer()
  {
    if (m_buffer != VK_NULL_HANDLE) {
      vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
    }
  }

  Buffer& Buffer::operator=(Buffer&& other) noexcept
  {
    std::swap(m_allocator, other.m_allocator);
    std::swap(m_buffer, other.m_buffer);
    std::swap(m_allocation, other.m_allocation);
    std::swap(m_count, other.m_count);
    std::swap(m_size, other.m_size);
    std::swap(m_type, other.m_type);
    std::swap(m_usage, other.m_usage);
    return *this;
  }

  void Buffer::update(std::size_t count, std::size_t member_size, const void* data, Renderer* renderer)
  {
    const std::size_t total_size = count * member_size;
    assert(total_size <= m_size);

    switch (m_type) {
      case BufferType::Host:
        update_host_buffer(total_size, data);
        break;
      case BufferType::Device:
        update_device_buffer(total_size, data, renderer);
        break;
    }

    m_count = count;
  }

  void Buffer::set_debug_name(const std::string& name) const
  {
    VmaAllocatorInfo info;
    vmaGetAllocatorInfo(m_allocator, &info);

    VkDebugUtilsObjectNameInfoEXT name_info = {};
    name_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
    name_info.objectType = VK_OBJECT_TYPE_BUFFER;
    name_info.objectHandle = details::to_debug_handle(m_buffer);
    name_info.pObjectName = name.c_str();

    vkSetDebugUtilsObjectNameEXT(info.device, &name_info);
  }

  void Buffer::create_host_buffer(BufferUsage usage, std::size_t total_size, const void* data)
  {
    VkBufferCreateInfo buffer_info = {};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = static_cast<VkDeviceSize>(total_size);
    buffer_info.usage = static_cast<VkBufferUsageFlagBits>(usage);
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocation_info = {};
    allocation_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
    allocation_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;

    if (vmaCreateBuffer(m_allocator, &buffer_info, &allocation_info, &m_buffer, &m_allocation, nullptr) != VK_SUCCESS) {
      Log::error("Failed to allocate buffer.");
      throw std::runtime_error("Failed to allocate buffer.");
    }

    update_host_buffer(total_size, data);
  }

  void Buffer::create_device_buffer(BufferUsage usage, std::size_t total_size, const void* data, Renderer* renderer)
  {
    VkBufferCreateInfo buffer_info = {};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = static_cast<VkDeviceSize>(total_size);
    buffer_info.usage = static_cast<VkBufferUsageFlagBits>(usage) | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocation_info = {};
    allocation_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

    if (vmaCreateBuffer(m_allocator, &buffer_info, &allocation_info, &m_buffer, &m_allocation, nullptr) != VK_SUCCESS) {
      Log::error("Failed to allocate buffer.");
      throw std::runtime_error("Failed to allocate buffer.");
    }

    update_device_buffer(total_size, data, renderer);
  }

  void Buffer::update_host_buffer(std::size_t total_size, const void* data)
  {
    void* memory = nullptr;

    if (vmaMapMemory(m_allocator, m_allocation, &memory) != VK_SUCCESS) {
      Log::error("Failed to map memory.");
      throw std::runtime_error("Failed to map memory.");
    }

    std::memcpy(memory, data, total_size);
    vmaUnmapMemory(m_allocator, m_allocation);
  }

  void Buffer::update_device_buffer(std::size_t total_size, const void* data, Renderer* renderer)
  {
    // staging buffer

    VkBufferCreateInfo staging_buffer_info = {};
    staging_buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    staging_buffer_info.size = static_cast<VkDeviceSize>(total_size);
    staging_buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    staging_buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo staging_allocation_info = {};
    staging_allocation_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
    staging_allocation_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;

    VkBuffer staging_buffer = VK_NULL_HANDLE;
    VmaAllocation staging_allocation = nullptr;

    if (vmaCreateBuffer(m_allocator, &staging_buffer_info, &staging_allocation_info, &staging_buffer, &staging_allocation, nullptr) != VK_SUCCESS) {
      Log::error("Failed to allocate buffer.");
      throw std::runtime_error("Failed to allocate buffer.");
    }

    void* memory = nullptr;

    if (vmaMapMemory(m_allocator, staging_allocation, &memory) != VK_SUCCESS) {
      Log::error("Failed to map memory.");
      throw std::runtime_error("Failed to map memory.");
    }

    std::memcpy(memory, data, total_size);
    vmaUnmapMemory(m_allocator, staging_allocation);

    // commands

    auto command_buffer = renderer->current_memory_command_buffer();

    command_buffer.copy_buffer_to_buffer({ staging_buffer }, { m_buffer }, total_size);

    // destroy staging buffer

    renderer->defer_release_staging_buffer({ staging_buffer, staging_allocation });
  }

}
