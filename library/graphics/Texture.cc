// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Texture.h>
// clang-format on

#include <cassert>

#include <utility>

#include <gf2/Log.h>
#include <gf2/Renderer.h>

namespace gf {

  Texture::Texture(const std::filesystem::path& filename, Renderer* renderer)
  : Texture(Image(filename), renderer)
  {
  }

  Texture::Texture(const Image& image, Renderer* renderer)
  : m_allocator(renderer->m_allocator)
  {
    auto image_size = image.size();
    auto total_size = image.raw_size();

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

    std::memcpy(memory, image.raw_pixels(), total_size);
    vmaUnmapMemory(m_allocator, staging_allocation);

    // device image

    VkImageCreateInfo image_info = {};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.extent.width = static_cast<uint32_t>(image_size.w);
    image_info.extent.height = static_cast<uint32_t>(image_size.h);
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.format = VK_FORMAT_R8G8B8A8_SRGB;
    image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;

    VmaAllocationCreateInfo allocation_info = {};
    allocation_info.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    allocation_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

    if (vmaCreateImage(m_allocator, &image_info, &allocation_info, &m_image, &m_allocation, nullptr) != VK_SUCCESS) {
      Log::error("Failed to allocate image.");
      throw std::runtime_error("Failed to allocate image.");
    }

    auto command_buffer = renderer->current_memory_command_buffer();

    command_buffer.texture_layout_transition({ m_image }, LayoutTransition::UploadReady);
    command_buffer.copy_buffer_to_texture({ staging_buffer }, { m_image }, image_size);
    command_buffer.texture_layout_transition({ m_image }, LayoutTransition::ShaderReady);

    VmaAllocatorInfo info;
    vmaGetAllocatorInfo(m_allocator, &info);

    // image view

    VkImageViewCreateInfo view_info = {};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image = m_image;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = VK_FORMAT_R8G8B8A8_SRGB;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;

    if (vkCreateImageView(info.device, &view_info, nullptr, &m_image_view) != VK_SUCCESS) {
      Log::error("Failed to create texture image view.");
      throw std::runtime_error("Failed to create texture image view.");
    }

    // sampler

    VkSamplerCreateInfo sampler_info = {};
    sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_info.magFilter = VK_FILTER_LINEAR;
    sampler_info.minFilter = VK_FILTER_LINEAR;
    sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.anisotropyEnable = VK_FALSE;
    sampler_info.unnormalizedCoordinates = VK_FALSE;
    sampler_info.compareEnable = VK_FALSE;
    sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
    sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler_info.mipLodBias = 0.0f;
    sampler_info.minLod = 0.0f;
    sampler_info.maxLod = 0.0f;

    if (vkCreateSampler(info.device, &sampler_info, nullptr, &m_sampler) != VK_SUCCESS) {
      Log::error("Failed to create texture sampler.");
      throw std::runtime_error("Failed to create texture sampler.");
    }

    // destroy staging buffer

    renderer->defer_release_staging_buffer({ staging_buffer, staging_allocation });
  }

  Texture::Texture(const Bitmap& bitmap, Renderer* renderer)
  {
  }

  Texture::Texture(Texture&& other) noexcept
  : m_allocator(std::exchange(other.m_allocator, nullptr))
  , m_image(std::exchange(other.m_image, VK_NULL_HANDLE))
  , m_allocation(std::exchange(other.m_allocation, nullptr))
  , m_image_view(std::exchange(other.m_image_view, VK_NULL_HANDLE))
  , m_sampler(std::exchange(other.m_sampler, VK_NULL_HANDLE))
  {
  }

  Texture::~Texture()
  {
    assert(m_allocator);
    VmaAllocatorInfo info = {};
    vmaGetAllocatorInfo(m_allocator, &info);

    if (m_sampler != VK_NULL_HANDLE) {
      vkDestroySampler(info.device, m_sampler, nullptr);
    }

    if (m_image_view != VK_NULL_HANDLE) {
      vkDestroyImageView(info.device, m_image_view, nullptr);
    }

    if (m_image != VK_NULL_HANDLE) {
      vmaDestroyImage(m_allocator, m_image, m_allocation);
    }
  }

  Texture& Texture::operator=(Texture&& other) noexcept
  {
    std::swap(m_allocator, other.m_allocator);
    std::swap(m_image, other.m_image);
    std::swap(m_allocation, other.m_allocation);
    std::swap(m_image_view, other.m_image_view);
    std::swap(m_sampler, other.m_sampler);
    return *this;
  }

}