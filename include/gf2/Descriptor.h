// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_DESCRIPTOR_H
#define GF_DESCRIPTOR_H

#include <type_traits>

#include "GraphicsApi.h"
#include "ShaderStage.h"
#include "Vulkan.h"

namespace gf {
  class Texture;

  // NOLINTNEXTLINE(performance-enum-size)
  enum class DescriptorType : std::underlying_type_t<VkDescriptorType> {
    Sampler = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
  };

  struct GF_GRAPHICS_API DescriptorBinding {
    uint32_t binding = 0;
    DescriptorType type = DescriptorType::Sampler;
    ShaderStage stage = ShaderStage::Vertex;
  };

  class GF_GRAPHICS_API Descriptor {
  public:

    void write(uint32_t binding, const Texture& texture);

  private:
    friend class BasicRenderer;
    friend class RenderCommandBuffer;

    Descriptor(VkDevice device, VkDescriptorSet descriptor)
    : m_device(device)
    , m_descriptor(descriptor)
    {
    }

    VkDevice m_device = VK_NULL_HANDLE;
    VkDescriptorSet m_descriptor = VK_NULL_HANDLE;
  };

}

#endif // GF_DESCRIPTOR_H
