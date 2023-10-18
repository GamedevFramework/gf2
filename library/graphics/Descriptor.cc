// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Descriptor.h>
// clang-format on

#include <utility>

#include <gf2/core/Log.h>

#include <gf2/graphics/Renderer.h>
#include <gf2/graphics/Texture.h>

namespace gf {

  /*
   * DescriptorLayout
   */

  DescriptorLayout::DescriptorLayout(Span<const DescriptorBinding> bindings, Renderer* renderer)
  : m_device(renderer->m_device)
  {
    assert(!bindings.empty());

    std::vector<VkDescriptorSetLayoutBinding> descriptor_set_layout_bindings;

    for (auto descriptor_binding : bindings) {
      VkDescriptorSetLayoutBinding descriptor_set_layout_binding = {};
      descriptor_set_layout_binding.binding = descriptor_binding.binding;
      descriptor_set_layout_binding.descriptorType = static_cast<VkDescriptorType>(descriptor_binding.type);
      descriptor_set_layout_binding.descriptorCount = 1;
      descriptor_set_layout_binding.stageFlags = static_cast<VkShaderStageFlags>(descriptor_binding.stage);

      descriptor_set_layout_bindings.push_back(descriptor_set_layout_binding);
    }

    VkDescriptorSetLayoutCreateInfo descriptor_set_layout_info = {};
    descriptor_set_layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptor_set_layout_info.bindingCount = static_cast<uint32_t>(descriptor_set_layout_bindings.size());
    descriptor_set_layout_info.pBindings = descriptor_set_layout_bindings.data();

    if (vkCreateDescriptorSetLayout(m_device, &descriptor_set_layout_info, nullptr, &m_layout) != VK_SUCCESS) {
      Log::error("Failed to create descriptor set layout.");
      throw std::runtime_error("Failed to create descriptor set layout.");
    }
  }

  DescriptorLayout::DescriptorLayout(DescriptorLayout&& other) noexcept
  : m_device(std::exchange(other.m_device, VK_NULL_HANDLE))
  , m_layout(std::exchange(other.m_layout, VK_NULL_HANDLE))
  {
  }

  DescriptorLayout::~DescriptorLayout()
  {
    if (m_layout != VK_NULL_HANDLE) {
      vkDestroyDescriptorSetLayout(m_device, m_layout, nullptr);
    }
  }

  DescriptorLayout& DescriptorLayout::operator=(DescriptorLayout&& other) noexcept
  {
    std::swap(m_device, other.m_device);
    std::swap(m_layout, other.m_layout);
    return *this;
  }

  /*
   * Descriptor
   */

  void Descriptor::write(uint32_t binding, const Texture* texture)
  {
    VkDescriptorImageInfo image_info = {};
    image_info.sampler = texture->m_sampler;
    image_info.imageView = texture->m_image_view;
    image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkWriteDescriptorSet write = {};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = m_descriptor;
    write.dstBinding = binding;
    write.dstArrayElement = 0;
    write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write.descriptorCount = 1;
    write.pImageInfo = &image_info;

    vkUpdateDescriptorSets(m_device, 1, &write, 0, nullptr);
  }

  void Descriptor::write(uint32_t binding, const Buffer* buffer)
  {
    VkDescriptorBufferInfo buffer_info = {};
    buffer_info.buffer = buffer->m_buffer;
    buffer_info.offset = 0;
    buffer_info.range = VK_WHOLE_SIZE;

    VkWriteDescriptorSet write = {};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = m_descriptor;
    write.dstBinding = binding;
    write.dstArrayElement = 0;
    write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    write.descriptorCount = 1;
    write.pBufferInfo = &buffer_info;

    vkUpdateDescriptorSets(m_device, 1, &write, 0, nullptr);
  }

}
