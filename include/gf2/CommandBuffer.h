#ifndef GF_COMMAND_BUFFER_H
#define GF_COMMAND_BUFFER_H

#include "Color.h"
#include "GraphicsApi.h"
#include "Rect.h"
#include "RenderTarget.h"
#include "Vulkan.h"

namespace gf {
  class Buffer;
  class Pipeline;

  class GF_GRAPHICS_API CommandBuffer {
  public:

    void begin_rendering(RenderTarget target, Color clear_color = Black) const;
    void end_rendering() const;

    void set_viewport(RectF viewport) const;
    void set_scissor(RectI scissor) const;

    void bind_pipeline(const Pipeline* pipeline) const;
    void bind_vertex_buffer(const Buffer* buffer, std::size_t offset = 0) const;
    void bind_index_buffer(const Buffer* buffer, std::size_t offset = 0) const;

    void draw(std::size_t vertex_count) const;
    void draw_indexed(std::size_t index_count) const;

  private:
    friend class BasicRenderer;

    CommandBuffer(VkCommandBuffer buffer)
    : m_command_buffer(buffer)
    {
    }

    VkCommandBuffer m_command_buffer;
  };

}

#endif // GF_COMMAND_BUFFER_H
