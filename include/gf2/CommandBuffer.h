#ifndef GF_COMMAND_BUFFER_H
#define GF_COMMAND_BUFFER_H

#include <cstdint>

#include "Color.h"
#include "Descriptor.h"
#include "GraphicsApi.h"
#include "Rect.h"
#include "RenderTarget.h"
#include "Vec2.h"
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

    void bind_descriptor(const Pipeline* pipeline, Descriptor descriptor) const;

    void draw(std::size_t vertex_count) const;
    void draw_indexed(std::size_t index_count) const;

  private:
    friend class BasicRenderer;

    CommandBuffer(VkCommandBuffer buffer)
    : m_command_buffer(buffer)
    {
    }

    VkCommandBuffer m_command_buffer = VK_NULL_HANDLE; // non-owning
  };

  class BufferReference {
  public:

  private:
    friend class MemoryCommandBuffer;
    friend class Buffer;
    friend class Texture;

    BufferReference(VkBuffer buffer)
    : m_buffer(buffer)
    {
    }

    VkBuffer m_buffer = VK_NULL_HANDLE;
  };

  class TextureReference {
  public:

  private:
    friend class MemoryCommandBuffer;
    friend class Texture;

    TextureReference(VkImage image)
    : m_image(image)
    {
    }

    VkImage m_image = VK_NULL_HANDLE;
  };

  enum class LayoutTransition : uint8_t {
    UploadReady,
    ShaderReady,
  };

  class GF_GRAPHICS_API MemoryCommandBuffer {
  public:

    void copy_buffer_to_buffer(BufferReference source, BufferReference destination, std::size_t size);
    void copy_buffer_to_texture(BufferReference source, TextureReference destination, Vec2I size);

    void texture_layout_transition(TextureReference texture, LayoutTransition transition);

  private:
    friend class MemoryAllocator;
    friend class BasicRenderer;

    MemoryCommandBuffer(VkCommandBuffer buffer)
    : m_command_buffer(buffer)
    {
    }

    VkCommandBuffer m_command_buffer = VK_NULL_HANDLE; // non-owning
  };

}

#endif // GF_COMMAND_BUFFER_H
