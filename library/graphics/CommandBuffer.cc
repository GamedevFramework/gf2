// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/CommandBuffer.h>
// clang-format on

#include <cassert>

#include <gf2/Buffer.h>
#include <gf2/Pipeline.h>

namespace gf {

  void CommandBuffer::begin_rendering(RenderTarget target, Color clear_color)
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

  void CommandBuffer::end_rendering()
  {
    vkCmdEndRendering(m_command_buffer);
  }

  void CommandBuffer::set_viewport(RectF viewport)
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

  void CommandBuffer::set_scissor(RectI scissor)
  {
    VkRect2D device_scissor;
    device_scissor.offset.x = scissor.offset.x;
    device_scissor.offset.y = scissor.offset.y;
    device_scissor.extent.width = static_cast<uint32_t>(scissor.extent.w);
    device_scissor.extent.height = static_cast<uint32_t>(scissor.extent.h);
    vkCmdSetScissor(m_command_buffer, 0, 1, &device_scissor);
  }

  void CommandBuffer::bind_pipeline(const Pipeline* pipeline)
  {
    assert(pipeline);
    vkCmdBindPipeline(m_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->m_pipeline);
  }

  void CommandBuffer::bind_vertex_buffer(const Buffer* buffer, std::size_t offset)
  {
    assert(buffer);
    VkDeviceSize device_offset = static_cast<VkDeviceSize>(offset);
    vkCmdBindVertexBuffers(m_command_buffer, 0, 1, &buffer->m_buffer, &device_offset);
  }

  void CommandBuffer::draw(std::size_t vertex_count)
  {
    vkCmdDraw(m_command_buffer, static_cast<uint32_t>(vertex_count), 1, 0, 0);
  }

}