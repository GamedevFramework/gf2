#ifndef GF_GPU_COMMAND_BUFFER_H
#define GF_GPU_COMMAND_BUFFER_H

#include <cstdint>

#include <SDL3/SDL_gpu.h>

#include <gf2/core/Color.h>
#include <gf2/core/Mat3.h>
#include <gf2/core/Rect.h>
#include <gf2/core/Vec2.h>

#include "GpuShader.h"
#include "GraphicsApi.h"
#include "RenderTarget.h"

namespace gf {
  class GpuTexture;
  class GpuBuffer;
  class RenderPipeline;
  class RenderPipelineLayout;
  class GpuTransferBuffer;

  class GF_GRAPHICS_API GpuRenderPass {
  public:
    GpuRenderPass() = default;

    void set_viewport(RectF viewport) const;
    void set_scissor(RectI scissor) const;

    void bind_pipeline(const RenderPipeline* pipeline) const;
    void bind_vertex_buffer(const GpuBuffer* buffer, std::size_t offset = 0) const;
    void bind_index_buffer(const GpuBuffer* buffer, std::size_t offset = 0) const;

    void bind_texture(GpuShaderStage stage, uint32_t slot_index, GpuTexture* buffer);
    void bind_storage_buffer(GpuShaderStage stage, uint32_t slot_index, GpuBuffer* buffer);

    void draw(std::size_t vertex_count, std::size_t first_vertex = 0) const;
    void draw_indexed(std::size_t index_count, std::size_t first_index = 0, std::ptrdiff_t vertex_offset = 0) const;

  private:
    friend class GpuCommandBuffer;

    GpuRenderPass(SDL_GPURenderPass* render_pass)
    : m_render_pass(render_pass)
    {
    }

    SDL_GPURenderPass* m_render_pass = nullptr;
  };

  class GF_GRAPHICS_API GpuCopyPass {
  public:
    GpuCopyPass() = default;

    void copy_buffer_to_buffer(GpuTransferBuffer* source, GpuBuffer* destination, std::size_t size);
    void copy_buffer_to_texture(GpuTransferBuffer* source, GpuTexture* destination, Vec2I size);

  private:
    friend class MemoryAllocator;
    friend class RenderManager;
    friend class GpuCommandBuffer;

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

#endif // GF_GPU_COMMAND_BUFFER_H
