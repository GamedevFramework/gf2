#ifndef GF_COMMAND_BUFFER_H
#define GF_COMMAND_BUFFER_H

#include <cstdint>

#include <gf2/core/Color.h>
#include <gf2/core/Mat3.h>
#include <gf2/core/Rect.h>
#include <gf2/core/Vec2.h>

#include "BufferReference.h"
#include "Descriptor.h"
#include "GraphicsApi.h"
#include "RenderTarget.h"
#include "TextureReference.h"

namespace gf {
  class Buffer;
  class RenderPipeline;
  class RenderPipelineLayout;

  class GF_GRAPHICS_API RenderCommandBuffer {
  public:

    void set_viewport(RectF viewport) const;
    void set_scissor(RectI scissor) const;

    void bind_pipeline(const RenderPipeline* pipeline) const;
    void bind_vertex_buffer(const Buffer* buffer, std::size_t offset = 0) const;
    void bind_index_buffer(const Buffer* buffer, std::size_t offset = 0) const;

    void bind_descriptor(const RenderPipelineLayout* pipeline, uint32_t set, Descriptor descriptor) const;

    void push_constant(const RenderPipelineLayout* pipeline, ShaderStage stage, std::size_t size, const void* data) const;
    void push_constant(const RenderPipelineLayout* pipeline, ShaderStage stage, const Mat3F& data) const;

    void draw(std::size_t vertex_count, std::size_t first_vertex = 0) const;
    void draw_indexed(std::size_t index_count, std::size_t first_index = 0, std::ptrdiff_t vertex_offset = 0) const;

  private:
    friend class CommandBuffer;

  };

  enum class Layout : uint8_t {
    Undefined,
    Upload,
    Shader,
    Target,
  };

  class GF_GRAPHICS_API MemoryCommandBuffer {
  public:

    void copy_buffer_to_buffer(BufferReference source, BufferReference destination, std::size_t size);
    void copy_buffer_to_texture(BufferReference source, TextureReference destination, Vec2I size);

    void texture_layout_transition(TextureReference texture, Layout from, Layout to);

  private:
    friend class MemoryAllocator;
    friend class RenderManager;
    friend class CommandBuffer;

  };

  class GF_GRAPHICS_API CommandBuffer {
  public:

    RenderCommandBuffer begin_rendering(RenderTarget target, Color clear_color = Black) const;
    void end_rendering(RenderCommandBuffer buffer) const;

    MemoryCommandBuffer begin_memory() const;
    void end_memory(MemoryCommandBuffer buffer) const;

  private:
    friend class RenderManager;

  };

}

#endif // GF_COMMAND_BUFFER_H
