// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_DESCRIPTOR_H
#define GF_DESCRIPTOR_H

#include <type_traits>

#include <vulkan/vulkan.h>

#include <gf2/core/Span.h>

#include "GraphicsApi.h"
#include "ShaderStage.h"

namespace gf {
  class Buffer;
  class Texture;
  class RenderManager;

  // NOLINTNEXTLINE(performance-enum-size)
  enum class DescriptorType : std::underlying_type_t<VkDescriptorType> {
    Sampler = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    Buffer = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
  };

  struct GF_GRAPHICS_API DescriptorBinding {
    uint32_t binding = 0;
    DescriptorType type = DescriptorType::Sampler;
    ShaderStage stage = ShaderStage::Vertex;
  };

  class GF_GRAPHICS_API DescriptorLayout {
  public:
    DescriptorLayout() = default;
    DescriptorLayout(Span<const DescriptorBinding> bindings, RenderManager* render_manager);
    DescriptorLayout(const DescriptorLayout&) = delete;
    DescriptorLayout(DescriptorLayout&& other) noexcept;
    ~DescriptorLayout();

    DescriptorLayout& operator=(const DescriptorLayout&) = delete;
    DescriptorLayout& operator=(DescriptorLayout&& other) noexcept;

  private:
    friend class RenderManager;
    friend class RenderPipelineLayoutBuilder;

    VkDevice m_device = VK_NULL_HANDLE; // non-owning
    VkDescriptorSetLayout m_layout = VK_NULL_HANDLE;
  };

  class GF_GRAPHICS_API Descriptor {
  public:

    void write(uint32_t binding, const Texture* texture);
    void write(uint32_t binding, const Buffer* buffer);

  private:
    friend class RenderManager;
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
