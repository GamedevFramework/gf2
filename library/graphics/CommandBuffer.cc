// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/CommandBuffer.h>

#include <cassert>
#include <cstddef>

#include <gf2/graphics/Buffer.h>
#include <gf2/graphics/RenderPipeline.h>

namespace gf {

  /*
   * CommandBuffer
   */

  RenderCommandBuffer CommandBuffer::begin_rendering(RenderTarget target, Color clear_color) const
  {

    return { };
  }

  void CommandBuffer::end_rendering([[maybe_unused]] RenderCommandBuffer buffer) const
  {
  }

  MemoryCommandBuffer CommandBuffer::begin_memory() const
  {
    return { };
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void CommandBuffer::end_memory([[maybe_unused]] MemoryCommandBuffer buffer) const
  {
  }

  /*
   * RenderCommandBuffer
   */

  void RenderCommandBuffer::set_viewport(RectF viewport) const
  {
  }

  void RenderCommandBuffer::set_scissor(RectI scissor) const
  {
  }

  void RenderCommandBuffer::bind_pipeline(const RenderPipeline* pipeline) const
  {
    assert(pipeline);
  }

  void RenderCommandBuffer::bind_vertex_buffer(const Buffer* buffer, std::size_t offset) const
  {
    assert(buffer);
  }

  void RenderCommandBuffer::bind_index_buffer(const Buffer* buffer, std::size_t offset) const
  {
    assert(buffer);
  }

  void RenderCommandBuffer::bind_descriptor(const RenderPipelineLayout* pipeline, uint32_t set, Descriptor descriptor) const
  {
    assert(pipeline);
  }

  void RenderCommandBuffer::push_constant(const RenderPipelineLayout* pipeline, ShaderStage stage, std::size_t size, const void* data) const
  {
    assert(pipeline);
    assert(size % 4 == 0);
  }

  void RenderCommandBuffer::push_constant(const RenderPipelineLayout* pipeline, ShaderStage stage, const Mat3F& data) const
  {
    auto aligned_data = compute_aligned(data);
    push_constant(pipeline, stage, sizeof(float) * aligned_data.size(), aligned_data.data());
  }

  void RenderCommandBuffer::draw(std::size_t vertex_count, std::size_t first_vertex) const
  {
  }

  void RenderCommandBuffer::draw_indexed(std::size_t index_count, std::size_t first_index, std::ptrdiff_t vertex_offset) const
  {
  }

  /*
   * MemoryCommandBuffer
   */

  void MemoryCommandBuffer::copy_buffer_to_buffer(BufferReference source, BufferReference destination, std::size_t size)
  {
  }

  void MemoryCommandBuffer::copy_buffer_to_texture(BufferReference source, TextureReference destination, Vec2I size)
  {
  }

  void MemoryCommandBuffer::texture_layout_transition(TextureReference texture, Layout from, Layout to)
  {
  }

}
