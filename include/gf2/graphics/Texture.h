// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TEXTURE_H
#define GF_TEXTURE_H

#include <filesystem>

#include <gf2/core/Bitmap.h>
#include <gf2/core/Image.h>

#include "GraphicsApi.h"
#include "RenderTarget.h"
#include "TextureReference.h"
#include "Vulkan.h"

#include <vk_mem_alloc.h>

namespace gf {
  class Renderer;

  class GF_GRAPHICS_API Texture {
  public:
    Texture() = default;
    Texture(const std::filesystem::path& filename, Renderer* renderer);
    Texture(const Image& image, Renderer* renderer);
    Texture(const Bitmap& bitmap, Renderer* renderer);
    Texture(Vec2I size, Renderer* renderer);

    Texture(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    ~Texture();

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&& other) noexcept;

    Vec2I size() const
    {
      return m_image_size;
    }

    RenderTarget as_render_target() const;

    operator TextureReference() const;


  private:
    friend class Descriptor;

    void upload_data(std::size_t raw_size, const void* raw_data, VkFormat format, Renderer* renderer);
    void create_image_view_and_sampler(VkFormat format);

    Vec2I m_image_size = { 0, 0 };
    VmaAllocator m_allocator = nullptr; // non-owning
    VkImage m_image = VK_NULL_HANDLE;
    VmaAllocation m_allocation = nullptr;
    VkImageView m_image_view = VK_NULL_HANDLE;
    VkSampler m_sampler = VK_NULL_HANDLE;
  };

}

#endif // GF_TEXTURE_H
