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

  RenderPass CommandBuffer::begin_render_pass(RenderTarget target, Color clear_color) const
  {
    assert(m_command_buffer);

    SDL_GPUColorTargetInfo info = {};
    info.texture = target.m_texture;
    info.clear_color = { clear_color.r, clear_color.g, clear_color.b, clear_color.a };
    info.load_op = SDL_GPU_LOADOP_CLEAR;
    info.store_op = SDL_GPU_STOREOP_STORE;

    SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(m_command_buffer, &info, 1, nullptr);
    return { render_pass };
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void CommandBuffer::end_render_pass(RenderPass pass) const
  {
    assert(m_command_buffer);
    SDL_EndGPURenderPass(pass.m_render_pass);
  }

  CopyPass CommandBuffer::begin_copy_pass() const
  {
    assert(m_command_buffer);
    SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(m_command_buffer);
    return { copy_pass };
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void CommandBuffer::end_copy_pass([[maybe_unused]] CopyPass pass) const
  {
    assert(m_command_buffer);
    SDL_EndGPUCopyPass(pass.m_copy_pass);
  }

  /*
   * RenderCommandBuffer
   */

  void RenderPass::set_viewport(RectF viewport) const
  {
    assert(m_render_pass);
    const SDL_GPUViewport raw_viewport = { viewport.offset.x, viewport.offset.y, viewport.extent.w, viewport.extent.h, 0.0f, 0.0f };
    SDL_SetGPUViewport(m_render_pass, &raw_viewport);
  }

  void RenderPass::set_scissor(RectI scissor) const
  {
    assert(m_render_pass);
    const SDL_Rect raw_scissor = { scissor.offset.x, scissor.offset.y, scissor.extent.w, scissor.extent.h };
    SDL_SetGPUScissor(m_render_pass, &raw_scissor);
  }

  void RenderPass::bind_pipeline(const RenderPipeline* pipeline) const
  {
    assert(m_render_pass);
    assert(pipeline);
    // TODO
  }

  void RenderPass::bind_vertex_buffer(const Buffer* buffer, std::size_t offset) const
  {
    assert(m_render_pass);
    assert(buffer);
    const SDL_GPUBufferBinding binding = { nullptr /* TODO */, static_cast<Uint32>(offset) };
    SDL_BindGPUVertexBuffers(m_render_pass, 0, &binding, 1);
  }

  void RenderPass::bind_index_buffer(const Buffer* buffer, std::size_t offset) const
  {
    assert(m_render_pass);
    assert(buffer);
    const SDL_GPUBufferBinding binding = { nullptr /* TODO */, static_cast<Uint32>(offset) };
    SDL_BindGPUIndexBuffer(m_render_pass, &binding, SDL_GPU_INDEXELEMENTSIZE_16BIT);
  }

  void RenderPass::bind_descriptor(const RenderPipelineLayout* pipeline, uint32_t set, Descriptor descriptor) const
  {
    assert(m_render_pass);
    assert(pipeline);
    // TODO
  }

  void RenderPass::push_constant(const RenderPipelineLayout* pipeline, ShaderStage stage, std::size_t size, const void* data) const
  {
    assert(m_render_pass);
    assert(pipeline);
    assert(size % 4 == 0);
    // TODO
  }

  void RenderPass::push_constant(const RenderPipelineLayout* pipeline, ShaderStage stage, const Mat3F& data) const
  {
    assert(m_render_pass);
    auto aligned_data = compute_aligned(data);
    push_constant(pipeline, stage, sizeof(float) * aligned_data.size(), aligned_data.data());
  }

  void RenderPass::draw(std::size_t vertex_count, std::size_t first_vertex) const
  {
    assert(m_render_pass);
    SDL_DrawGPUPrimitives(m_render_pass, vertex_count, 1, first_vertex, 0);
  }

  void RenderPass::draw_indexed(std::size_t index_count, std::size_t first_index, std::ptrdiff_t vertex_offset) const
  {
    assert(m_render_pass);
    SDL_DrawGPUIndexedPrimitives(m_render_pass, index_count, 1, first_index, static_cast<Sint32>(vertex_offset), 0);
  }

  /*
   * MemoryCommandBuffer
   */

  void CopyPass::copy_buffer_to_buffer(BufferReference source, BufferReference destination, std::size_t size)
  {
    assert(m_copy_pass);
    // TODO
  }

  void CopyPass::copy_buffer_to_texture(BufferReference source, TextureReference destination, Vec2I size)
  {
    assert(m_copy_pass);
    // TODO
  }

}
