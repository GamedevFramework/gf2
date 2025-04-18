// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Buffer.h>

#include <cstring>

#include <utility>

#include <volk.h>

#include <gf2/core/Log.h>

#include <gf2/graphics/RenderManager.h>

namespace gf {

  Buffer::Buffer(BufferType type, BufferUsage usage, std::size_t size, std::size_t member_size, const void* data, RenderManager* render_manager)
  : m_allocator(render_manager->m_allocator)
  , m_size(size)
  , m_memory_size(size * member_size)
  , m_type(type)
  , m_usage(usage)
  {
    if (m_size == 0) {
      return;
    }

    switch (type) {
      case BufferType::Host:
        create_host_buffer(usage, m_memory_size, data);
        break;
      case BufferType::Device:
        create_device_buffer(usage, m_memory_size, data, render_manager);
        break;
    }
  }

  Buffer::Buffer(Buffer&& other) noexcept
  : m_allocator(std::exchange(other.m_allocator, nullptr))
  , m_buffer(std::exchange(other.m_buffer, VK_NULL_HANDLE))
  , m_allocation(std::exchange(other.m_allocation, nullptr))
  , m_size(other.m_size)
  , m_memory_size(other.m_memory_size)
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
    std::swap(m_size, other.m_size);
    std::swap(m_memory_size, other.m_memory_size);
    std::swap(m_type, other.m_type);
    std::swap(m_usage, other.m_usage);
    return *this;
  }

  void Buffer::update(std::size_t size, std::size_t member_size, const void* data, RenderManager* render_manager)
  {
    const std::size_t total_size = size * member_size;
    assert(total_size <= m_memory_size);
    m_size = size;

    if (size == 0) {
      return;
    }

    switch (m_type) {
      case BufferType::Host:
        update_host_buffer(total_size, data);
        break;
      case BufferType::Device:
        update_device_buffer(total_size, data, render_manager);
        break;
    }
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
      Log::fatal("Failed to allocate buffer (total size: {}).", total_size);
    }

    update_host_buffer(total_size, data);
  }

  void Buffer::create_device_buffer(BufferUsage usage, std::size_t total_size, const void* data, RenderManager* render_manager)
  {
    VkBufferCreateInfo buffer_info = {};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = static_cast<VkDeviceSize>(total_size);
    buffer_info.usage = static_cast<VkBufferUsageFlagBits>(usage) | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocation_info = {};
    allocation_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

    if (vmaCreateBuffer(m_allocator, &buffer_info, &allocation_info, &m_buffer, &m_allocation, nullptr) != VK_SUCCESS) {
      Log::fatal("Failed to allocate buffer (total size: {}).", total_size);
    }

    update_device_buffer(total_size, data, render_manager);
  }

  void Buffer::update_host_buffer(std::size_t total_size, const void* data)
  {
    if (vmaCopyMemoryToAllocation(m_allocator, data, m_allocation, 0, total_size) != VK_SUCCESS) {
      Log::fatal("Failed copy memory to buffer.");
    }
  }

  void Buffer::update_device_buffer(std::size_t total_size, const void* data, RenderManager* render_manager)
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
      Log::fatal("Failed to allocate buffer.");
    }

    if (vmaCopyMemoryToAllocation(m_allocator, data, staging_allocation, 0, total_size) != VK_SUCCESS) {
      Log::fatal("Failed copy memory to buffer.");
    }

    // commands

    auto command_buffer = render_manager->current_memory_command_buffer();

    command_buffer.copy_buffer_to_buffer({ staging_buffer }, { m_buffer }, total_size);

    // destroy staging buffer

    render_manager->defer_release_staging_buffer({ staging_buffer, staging_allocation });
  }

}
