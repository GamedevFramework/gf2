// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TEXTURE_H
#define GF_TEXTURE_H

#include <filesystem>
#include <string>
#include <type_traits>

#include <vk_mem_alloc.h>

#include <gf2/core/Bitmap.h>
#include <gf2/core/Flags.h>
#include <gf2/core/Image.h>

#include "BufferReference.h"
#include "Format.h"
#include "GraphicsApi.h"
#include "RenderTarget.h"
#include "TextureReference.h"
#include "Vulkan.h"

namespace gf {
  class Renderer;

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
    using Context = Renderer*;

    Texture() = default;
    Texture(const std::filesystem::path& filename, Renderer* renderer);
    Texture(const Image& image, Renderer* renderer);
    Texture(const Bitmap& bitmap, Renderer* renderer);
    Texture(Vec2I size, Renderer* renderer);
    Texture(Vec2I size, Flags<TextureUsage> usage, Format format, Renderer* renderer);

    Texture(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    ~Texture();

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&& other) noexcept;

    void set_debug_name(const std::string& name) const;

    void update(const Image& image, Renderer* renderer);
    void update(const Bitmap& bitmap, Renderer* renderer);

    Vec2I size() const
    {
      return m_image_size;
    }

    RenderTarget as_render_target() const;

    operator TextureReference() const;

  private:
    friend class Descriptor;

    StagingBufferReference create_staging_buffer(std::size_t raw_size, const void* raw_data);
    void create_image_view_and_sampler(Format format, Renderer* renderer);
    void compute_memory_operations(StagingBufferReference staging, Renderer* renderer);

    Vec2I m_image_size = { 0, 0 };
    VmaAllocator m_allocator = nullptr; // non-owning
    VkImage m_image = VK_NULL_HANDLE;
    VmaAllocation m_allocation = nullptr;
    VkImageView m_image_view = VK_NULL_HANDLE;
    VkSampler m_sampler = VK_NULL_HANDLE;
    Flags<TextureUsage> m_usage = None;
    Format m_format = Format::Undefined;
  };

}

#endif // GF_TEXTURE_H
