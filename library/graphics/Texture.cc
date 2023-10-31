// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Texture.h>
// clang-format on

#include <cassert>

#include <utility>

#include <volk.h>

#include <gf2/core/Log.h>

#include <gf2/graphics/RenderManager.h>

namespace gf {

  using namespace operators;

  Texture::Texture(const std::filesystem::path& filename, RenderManager* render_manager)
  : Texture(Image(filename), render_manager)
  {
  }

  Texture::Texture(const Image& image, RenderManager* render_manager)
  : Texture(image.size(), TextureUsage::TransferDestination | TextureUsage::Sampled, Format::Color8S, render_manager)
  {
    auto raw_size = image.raw_size();
    const auto* raw_data = image.raw_data();

    auto staging = create_staging_buffer(raw_size, raw_data);
    compute_memory_operations(staging, render_manager);
  }

  Texture::Texture(const Bitmap& bitmap, RenderManager* render_manager)
  : Texture(bitmap.size(), TextureUsage::TransferDestination | TextureUsage::Sampled, Format::Gray8U, render_manager)
  {
    auto raw_size = bitmap.raw_size();
    const auto* raw_data = bitmap.raw_data();

    auto staging = create_staging_buffer(raw_size, raw_data);
    compute_memory_operations(staging, render_manager);
  }

  Texture::Texture(Vec2I size, RenderManager* render_manager)
  : Texture(size, TextureUsage::ColorTarget | TextureUsage::Sampled, Format::RenderManager, render_manager)
  {
  }

  Texture::Texture(Vec2I size, Flags<TextureUsage> usage, Format format, RenderManager* render_manager)
  : m_image_size(size)
  , m_allocator(render_manager->m_allocator)
  , m_usage(usage)
  , m_format(format)
  {
    VkImageCreateInfo image_info = {};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.extent.width = static_cast<uint32_t>(m_image_size.w);
    image_info.extent.height = static_cast<uint32_t>(m_image_size.h);
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.format = format == Format::RenderManager ? render_manager->m_format : static_cast<VkFormat>(format);
    image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = usage.value();
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;

    VmaAllocationCreateInfo allocation_info = {};
    allocation_info.flags = usage.test(TextureUsage::ColorTarget) ? VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT : 0;
    allocation_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

    if (vmaCreateImage(m_allocator, &image_info, &allocation_info, &m_image, &m_allocation, nullptr) != VK_SUCCESS) {
      Log::fatal("Failed to allocate image.");
    }

    // image view and sampler

    create_image_view_and_sampler(format, render_manager);
  }

  Texture::Texture(Texture&& other) noexcept
  : m_image_size(other.m_image_size)
  , m_allocator(std::exchange(other.m_allocator, nullptr))
  , m_allocation(std::exchange(other.m_allocation, nullptr))
  , m_image(std::exchange(other.m_image, VK_NULL_HANDLE))
  , m_image_view(std::exchange(other.m_image_view, VK_NULL_HANDLE))
  , m_sampler(std::exchange(other.m_sampler, VK_NULL_HANDLE))
  {
  }

  Texture::~Texture()
  {
    if (m_allocator == nullptr) {
      assert(m_allocation == nullptr);
      assert(m_image == VK_NULL_HANDLE);
      assert(m_image_view == VK_NULL_HANDLE);
      assert(m_sampler == VK_NULL_HANDLE);
      return;
    }

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
    std::swap(m_image_size, other.m_image_size);
    std::swap(m_allocator, other.m_allocator);
    std::swap(m_allocation, other.m_allocation);
    std::swap(m_image, other.m_image);
    std::swap(m_image_view, other.m_image_view);
    std::swap(m_sampler, other.m_sampler);
    return *this;
  }

  void Texture::set_debug_name(const std::string& name) const
  {
    VmaAllocatorInfo info;
    vmaGetAllocatorInfo(m_allocator, &info);

    VkDebugUtilsObjectNameInfoEXT name_info = {};
    name_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
    name_info.pObjectName = name.c_str();

    name_info.objectType = VK_OBJECT_TYPE_IMAGE;
    name_info.objectHandle = details::to_debug_handle(m_image);

    vkSetDebugUtilsObjectNameEXT(info.device, &name_info);

    name_info.objectType = VK_OBJECT_TYPE_IMAGE_VIEW;
    name_info.objectHandle = details::to_debug_handle(m_image_view);

    vkSetDebugUtilsObjectNameEXT(info.device, &name_info);

    name_info.objectType = VK_OBJECT_TYPE_SAMPLER;
    name_info.objectHandle = details::to_debug_handle(m_sampler);

    vkSetDebugUtilsObjectNameEXT(info.device, &name_info);
  }

  void Texture::update(const Image& image, RenderManager* render_manager)
  {
    assert(image.size() == m_image_size);
    assert(m_format == Format::Color8S);
    assert(m_usage.test(TextureUsage::TransferDestination));

    auto raw_size = image.raw_size();
    const auto* raw_data = image.raw_data();

    auto staging = create_staging_buffer(raw_size, raw_data);
    compute_memory_operations(staging, render_manager);
  }

  void Texture::update(const Bitmap& bitmap, RenderManager* render_manager)
  {
    assert(bitmap.size() == m_image_size);
    assert(m_format == Format::Gray8U);
    assert(m_usage.test(TextureUsage::TransferDestination));

    auto raw_size = bitmap.raw_size();
    const auto* raw_data = bitmap.raw_data();

    auto staging = create_staging_buffer(raw_size, raw_data);
    compute_memory_operations(staging, render_manager);
  }

  RenderTarget Texture::as_render_target() const
  {
    const VkExtent2D extent = { static_cast<uint32_t>(m_image_size.w), static_cast<uint32_t>(m_image_size.h) };
    return { m_image_view, extent };
  }

  Texture::operator TextureReference() const
  {
    return { m_image };
  }

  StagingBufferReference Texture::create_staging_buffer(std::size_t raw_size, const void* raw_data)
  {
    VkBufferCreateInfo staging_buffer_info = {};
    staging_buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    staging_buffer_info.size = static_cast<VkDeviceSize>(raw_size);
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

    void* memory = nullptr;

    if (vmaMapMemory(m_allocator, staging_allocation, &memory) != VK_SUCCESS) {
      Log::fatal("Failed to map memory.");
    }

    std::memcpy(memory, raw_data, raw_size);
    vmaUnmapMemory(m_allocator, staging_allocation);

    return { staging_buffer, staging_allocation };
  }

  void Texture::create_image_view_and_sampler(Format format, RenderManager* render_manager)
  {
    VmaAllocatorInfo info;
    vmaGetAllocatorInfo(m_allocator, &info);

    // image view

    VkImageViewCreateInfo view_info = {};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image = m_image;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = format == Format::RenderManager ? render_manager->m_format : static_cast<VkFormat>(format);
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;

    if (vkCreateImageView(info.device, &view_info, nullptr, &m_image_view) != VK_SUCCESS) {
      Log::fatal("Failed to create texture image view.");
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
      Log::fatal("Failed to create texture sampler.");
    }
  }

  void Texture::compute_memory_operations(StagingBufferReference staging, RenderManager* render_manager)
  {
    auto command_buffer = render_manager->current_memory_command_buffer();

    command_buffer.texture_layout_transition({ m_image }, Layout::Undefined, Layout::Upload);
    command_buffer.copy_buffer_to_texture({ staging.m_buffer }, { m_image }, m_image_size);
    command_buffer.texture_layout_transition({ m_image }, Layout::Upload, Layout::Shader);

    render_manager->defer_release_staging_buffer(staging);
  }

}
