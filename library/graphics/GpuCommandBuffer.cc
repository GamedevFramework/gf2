// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/GpuCommandBuffer.h>

#include <cassert>
#include <cstddef>

#include <gf2/graphics/GpuBuffer.h>
#include <gf2/graphics/RenderPipeline.h>

namespace gf {

  /*
   * CommandBuffer
   */

  GpuRenderPass GpuCommandBuffer::begin_render_pass(RenderTarget target, Color clear_color) const
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
  void GpuCommandBuffer::end_render_pass(GpuRenderPass pass) const
  {
    assert(m_command_buffer);
    SDL_EndGPURenderPass(pass.m_render_pass);
  }

  GpuCopyPass GpuCommandBuffer::begin_copy_pass() const
  {
    assert(m_command_buffer);
    SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(m_command_buffer);
    return { copy_pass };
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void GpuCommandBuffer::end_copy_pass([[maybe_unused]] GpuCopyPass pass) const
  {
    assert(m_command_buffer);
    SDL_EndGPUCopyPass(pass.m_copy_pass);
  }

  /*
   * RenderCommandBuffer
   */

  void GpuRenderPass::set_viewport(RectF viewport) const
  {
    assert(m_render_pass);
    const SDL_GPUViewport raw_viewport = { viewport.offset.x, viewport.offset.y, viewport.extent.w, viewport.extent.h, 0.0f, 0.0f };
    SDL_SetGPUViewport(m_render_pass, &raw_viewport);
  }

  void GpuRenderPass::set_scissor(RectI scissor) const
  {
    assert(m_render_pass);
    const SDL_Rect raw_scissor = { scissor.offset.x, scissor.offset.y, scissor.extent.w, scissor.extent.h };
    SDL_SetGPUScissor(m_render_pass, &raw_scissor);
  }

  void GpuRenderPass::bind_pipeline(const RenderPipeline* pipeline) const
  {
    assert(m_render_pass);
    assert(pipeline);
    // TODO
  }

  void GpuRenderPass::bind_vertex_buffer(const GpuBuffer* buffer, std::size_t offset) const
  {
    assert(m_render_pass);
    assert(buffer);
    const SDL_GPUBufferBinding binding = { nullptr /* TODO */, static_cast<Uint32>(offset) };
    SDL_BindGPUVertexBuffers(m_render_pass, 0, &binding, 1);
  }

  void GpuRenderPass::bind_index_buffer(const GpuBuffer* buffer, std::size_t offset) const
  {
    assert(m_render_pass);
    assert(buffer);
    const SDL_GPUBufferBinding binding = { nullptr /* TODO */, static_cast<Uint32>(offset) };
    SDL_BindGPUIndexBuffer(m_render_pass, &binding, SDL_GPU_INDEXELEMENTSIZE_16BIT);
  }

  void GpuRenderPass::bind_texture(GpuShaderStage stage, uint32_t slot_index, GpuTexture* buffer)
  {
    assert(m_render_pass);

  }

  void GpuRenderPass::bind_storage_buffer(GpuShaderStage stage, uint32_t slot_index, GpuBuffer* buffer)
  {
    assert(m_render_pass);

  }

  void GpuRenderPass::draw(std::size_t vertex_count, std::size_t first_vertex) const
  {
    assert(m_render_pass);
    SDL_DrawGPUPrimitives(m_render_pass, vertex_count, 1, first_vertex, 0);
  }

  void GpuRenderPass::draw_indexed(std::size_t index_count, std::size_t first_index, std::ptrdiff_t vertex_offset) const
  {
    assert(m_render_pass);
    SDL_DrawGPUIndexedPrimitives(m_render_pass, index_count, 1, first_index, static_cast<Sint32>(vertex_offset), 0);
  }

  /*
   * MemoryCommandBuffer
   */

  void GpuCopyPass::copy_buffer_to_buffer(GpuTransferBuffer* source, GpuBuffer* destination, std::size_t size)
  {
    assert(m_copy_pass);
    // TODO
  }

  void GpuCopyPass::copy_buffer_to_texture(GpuTransferBuffer* source, GpuTexture* destination, Vec2I size)
  {
    assert(m_copy_pass);
    // TODO
  }

}
