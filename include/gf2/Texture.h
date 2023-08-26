// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TEXTURE_H
#define GF_TEXTURE_H

#include <filesystem>

#include "Bitmap.h"
#include "GraphicsApi.h"
#include "Image.h"
#include "Vulkan.h"

#include <vk_mem_alloc.h>

namespace gf {
  class Renderer;

  class GF_GRAPHICS_API Texture {
  public:
    Texture(const std::filesystem::path& filename, Renderer* renderer);
    Texture(const Image& image, Renderer* renderer);
    Texture(const Bitmap& bitmap, Renderer* renderer);

    Texture(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    ~Texture();

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&& other) noexcept;

  private:
    friend class Descriptor;

    VmaAllocator m_allocator = nullptr; // non-owning
    VkImage m_image = VK_NULL_HANDLE;
    VmaAllocation m_allocation = nullptr;
    VkImageView m_image_view = VK_NULL_HANDLE;
    VkSampler m_sampler = VK_NULL_HANDLE;
  };

}

#endif // GF_TEXTURE_H