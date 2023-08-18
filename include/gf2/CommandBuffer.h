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

    void begin_rendering(RenderTarget target, Color clear_color = Black);
    void end_rendering();

    void set_viewport(RectF viewport);
    void set_scissor(RectI scissor);

    void bind_pipeline(Pipeline* pipeline);
    void bind_vertex_buffer(Buffer* buffer, std::size_t offset = 0);

    void draw(std::size_t vertex_count);

  private:
    friend class Renderer;

    CommandBuffer(VkCommandBuffer buffer)
    : m_command_buffer(buffer)
    {
    }

    VkCommandBuffer m_command_buffer;
  };

}

#endif // GF_COMMAND_BUFFER_H
