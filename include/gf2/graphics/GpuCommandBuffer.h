#ifndef GF_COMMAND_BUFFER_H
#define GF_COMMAND_BUFFER_H

#include <cstdint>

#include <SDL3/SDL_gpu.h>

#include <gf2/core/Color.h>
#include <gf2/core/Mat3.h>
#include <gf2/core/Rect.h>
#include <gf2/core/Vec2.h>

#include "Descriptor.h"
#include "GraphicsApi.h"
#include "RenderTarget.h"
#include "GpuShader.h"

namespace gf {
  class Buffer;
  class RenderPipeline;
  class RenderPipelineLayout;
  class TransferBuffer;

  class GF_GRAPHICS_API GpuRenderPass {
  public:
    GpuRenderPass() = default;

    void set_viewport(RectF viewport) const;
    void set_scissor(RectI scissor) const;

    void bind_pipeline(const RenderPipeline* pipeline) const;
    void bind_vertex_buffer(const Buffer* buffer, std::size_t offset = 0) const;
    void bind_index_buffer(const Buffer* buffer, std::size_t offset = 0) const;

    void bind_descriptor(const RenderPipelineLayout* pipeline, uint32_t set, Descriptor descriptor) const;

    void push_constant(const RenderPipelineLayout* pipeline, GpuShaderStage stage, std::size_t size, const void* data) const;
    void push_constant(const RenderPipelineLayout* pipeline, GpuShaderStage stage, const Mat3F& data) const;

    void draw(std::size_t vertex_count, std::size_t first_vertex = 0) const;
    void draw_indexed(std::size_t index_count, std::size_t first_index = 0, std::ptrdiff_t vertex_offset = 0) const;

  private:
    friend class CommandBuffer;

    GpuRenderPass(SDL_GPURenderPass* render_pass)
    : m_render_pass(render_pass)
    {
    }

    SDL_GPURenderPass* m_render_pass = nullptr;
  };

  class GF_GRAPHICS_API GpuCopyPass {
  public:
    GpuCopyPass() = default;

    void copy_buffer_to_buffer(TransferBuffer* source, Buffer* destination, std::size_t size);
    void copy_buffer_to_texture(TransferBuffer* source, Texture* destination, Vec2I size);

  private:
    friend class MemoryAllocator;
    friend class RenderManager;
    friend class CommandBuffer;

    GpuCopyPass(SDL_GPUCopyPass* copy_pass)
    : m_copy_pass(copy_pass)
    {
    }

    SDL_GPUCopyPass* m_copy_pass = nullptr;
  };

  class GF_GRAPHICS_API GpuCommandBuffer {
  public:
    GpuCommandBuffer() = default;

    GpuRenderPass begin_render_pass(RenderTarget target, Color clear_color = Black) const;
    void end_render_pass(GpuRenderPass pass) const;

    GpuCopyPass begin_copy_pass() const;
    void end_copy_pass(GpuCopyPass pass) const;

  private:
    friend class RenderManager;

    GpuCommandBuffer(SDL_GPUCommandBuffer* command_buffer)
    : m_command_buffer(command_buffer)
    {
    }

    SDL_GPUCommandBuffer* m_command_buffer = nullptr;
  };

}

#endif // GF_COMMAND_BUFFER_H
