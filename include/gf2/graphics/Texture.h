// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_TEXTURE_H
#define GF_TEXTURE_H

#include <filesystem>
#include <string>
#include <type_traits>

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <gf2/core/Bitmap.h>
#include <gf2/core/Flags.h>
#include <gf2/core/Image.h>

#include "BufferReference.h"
#include "Format.h"
#include "GraphicsApi.h"
#include "RenderTarget.h"
#include "TextureReference.h"

namespace gf {
  class RenderManager;

  // NOLINTNEXTLINE(performance-enum-size)
  enum class TextureUsage : std::underlying_type_t<VkImageUsageFlagBits> {
    TransferSource = VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
    TransferDestination = VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    Sampled = VK_IMAGE_USAGE_SAMPLED_BIT,
    Storage = VK_IMAGE_USAGE_STORAGE_BIT,
    ColorTarget = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
  };

  template<>
  struct EnableBitmaskOperators<TextureUsage> : std::true_type {
  };

  class GF_GRAPHICS_API Texture {
  public:
    using Context = RenderManager*;

    Texture() = default;
    Texture(const std::filesystem::path& filename, RenderManager* render_manager);
    Texture(const Image& image, RenderManager* render_manager);
    Texture(const Bitmap& bitmap, RenderManager* render_manager);
    Texture(Vec2I size, RenderManager* render_manager);
    Texture(Vec2I size, Flags<TextureUsage> usage, Format format, RenderManager* render_manager);

    Texture(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    ~Texture();

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&& other) noexcept;

    void set_debug_name(const std::string& name) const;

    void update(const Image& image, RenderManager* render_manager);
    void update(const Bitmap& bitmap, RenderManager* render_manager);

    Vec2I size() const
    {
      return m_image_size;
    }

    RenderTarget as_render_target() const;

    operator TextureReference() const;

  private:
    friend class Descriptor;

    StagingBufferReference create_staging_buffer(std::size_t raw_size, const void* raw_data);
    void create_image_view_and_sampler(Format format, RenderManager* render_manager);
    void compute_memory_operations(StagingBufferReference staging, RenderManager* render_manager);

    Vec2I m_image_size = { 0, 0 };
    VmaAllocator m_allocator = nullptr; // non-owning
    VmaAllocation m_allocation = nullptr;
    VkImage m_image = VK_NULL_HANDLE;
    VkImageView m_image_view = VK_NULL_HANDLE;
    VkSampler m_sampler = VK_NULL_HANDLE;
    Flags<TextureUsage> m_usage = None;
    Format m_format = Format::Undefined;
  };

}

#endif // GF_TEXTURE_H
