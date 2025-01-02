// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_TEXTURE_REFERENCE_H
#define GF_TEXTURE_REFERENCE_H

#include <vulkan/vulkan.h>

namespace gf {

  class TextureReference {
  public:

  private:
    friend class MemoryCommandBuffer;
    friend class Texture;

    TextureReference(VkImage image)
    : m_image(image)
    {
    }

    VkImage m_image = VK_NULL_HANDLE;
  };

}

#endif // GF_TEXTURE_REFERENCE_H
