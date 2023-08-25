// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/CommandBuffer.h>
// clang-format on

#include <cassert>

#include <gf2/Buffer.h>
#include <gf2/Pipeline.h>

namespace gf {

  void CommandBuffer::begin_rendering(RenderTarget target, Color clear_color) const
  {
    VkRenderingAttachmentInfo color_attachment = {};
    color_attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    color_attachment.imageView = target.m_image_view;
    color_attachment.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.clearValue.color = {
      {clear_color.r, clear_color.g, clear_color.b, clear_color.a}
    };

    VkRenderingInfo rendering_info = {};
    rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    rendering_info.renderArea = {
      {0, 0},
      target.m_extent
    };
    rendering_info.layerCount = 1;
    rendering_info.colorAttachmentCount = 1;
    rendering_info.pColorAttachments = &color_attachment;
    rendering_info.pDepthAttachment = nullptr;
    rendering_info.pStencilAttachment = nullptr;

    vkCmdBeginRendering(m_command_buffer, &rendering_info);
  }

  void CommandBuffer::end_rendering() const
  {
    vkCmdEndRendering(m_command_buffer);
  }

  void CommandBuffer::set_viewport(RectF viewport) const
  {
    VkViewport device_viewport;
    device_viewport.x = viewport.offset.x;
    device_viewport.y = viewport.offset.y;
    device_viewport.width = viewport.extent.w;
    device_viewport.height = viewport.extent.h;
    device_viewport.minDepth = 0.0f;
    device_viewport.maxDepth = 1.0f;

    vkCmdSetViewport(m_command_buffer, 0, 1, &device_viewport);
  }

  void CommandBuffer::set_scissor(RectI scissor) const
  {
    VkRect2D device_scissor;
    device_scissor.offset.x = scissor.offset.x;
    device_scissor.offset.y = scissor.offset.y;
    device_scissor.extent.width = static_cast<uint32_t>(scissor.extent.w);
    device_scissor.extent.height = static_cast<uint32_t>(scissor.extent.h);
    vkCmdSetScissor(m_command_buffer, 0, 1, &device_scissor);
  }

  void CommandBuffer::bind_pipeline(const Pipeline* pipeline) const
  {
    assert(pipeline);
    vkCmdBindPipeline(m_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->m_pipeline);
  }

  void CommandBuffer::bind_vertex_buffer(const Buffer* buffer, std::size_t offset) const
  {
    assert(buffer);
    auto device_offset = static_cast<VkDeviceSize>(offset);
    vkCmdBindVertexBuffers(m_command_buffer, 0, 1, &buffer->m_buffer, &device_offset);
  }

  void CommandBuffer::bind_index_buffer(const Buffer* buffer, std::size_t offset) const
  {
    assert(buffer);
    auto device_offset = static_cast<VkDeviceSize>(offset);
    vkCmdBindIndexBuffer(m_command_buffer, buffer->m_buffer, device_offset, VK_INDEX_TYPE_UINT16);
  }

  void CommandBuffer::bind_descriptor(const Pipeline* pipeline, Descriptor descriptor) const
  {
    assert(pipeline);
    vkCmdBindDescriptorSets(m_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->m_pipeline_layout, 0, 1, &descriptor.m_descriptor, 0, nullptr);
  }

  void CommandBuffer::draw(std::size_t vertex_count) const
  {
    vkCmdDraw(m_command_buffer, static_cast<uint32_t>(vertex_count), 1, 0, 0);
  }

  void CommandBuffer::draw_indexed(std::size_t index_count) const
  {
    vkCmdDrawIndexed(m_command_buffer, static_cast<uint32_t>(index_count), 1, 0, 0, 0);
  }

  /*
   * MemoryCommandBuffer
   */

  void MemoryCommandBuffer::copy_buffer_to_buffer(BufferReference source, BufferReference destination, std::size_t size)
  {
    VkBufferCopy copy = {};
    copy.srcOffset = 0;
    copy.dstOffset = 0;
    copy.size = static_cast<VkDeviceSize>(size);
    vkCmdCopyBuffer(m_command_buffer, source.m_buffer, destination.m_buffer, 1, &copy);
  }

  void MemoryCommandBuffer::copy_buffer_to_texture(BufferReference source, TextureReference destination, Vec2I size)
  {
    VkBufferImageCopy copy = {};
    copy.bufferOffset = 0;
    copy.bufferRowLength = 0;
    copy.bufferImageHeight = 0;

    copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copy.imageSubresource.mipLevel = 0;
    copy.imageSubresource.baseArrayLayer = 0;
    copy.imageSubresource.layerCount = 1;

    copy.imageOffset = { 0, 0, 0 };
    copy.imageExtent = { static_cast<uint32_t>(size.w), static_cast<uint32_t>(size.h), 1 };

    vkCmdCopyBufferToImage(m_command_buffer, source.m_buffer, destination.m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);
  }

  void MemoryCommandBuffer::texture_layout_transition(TextureReference texture, LayoutTransition transition)
  {
    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

    switch (transition) {
      case LayoutTransition::UploadReady:
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        break;

      case LayoutTransition::ShaderReady:
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        break;
    }

    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    barrier.image = texture.m_image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    switch (transition) {
      case LayoutTransition::UploadReady:
        vkCmdPipelineBarrier(m_command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
        break;
      case LayoutTransition::ShaderReady:
        vkCmdPipelineBarrier(m_command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
        break;
    }
  }

}
