// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Descriptor.h>
// clang-format on

#include <utility>

#include <gf2/Texture.h>

namespace gf {

  void Descriptor::write(uint32_t binding, const Texture& texture)
  {
    VkDescriptorImageInfo image_info = {};
    image_info.sampler = texture.m_sampler;
    image_info.imageView = texture.m_image_view;
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

}
