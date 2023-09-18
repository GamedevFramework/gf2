#ifndef GF_COMMAND_BUFFER_H
#define GF_COMMAND_BUFFER_H

#include <cstdint>

#include <type_traits>

#include <gf2/core/Color.h>
#include <gf2/core/Rect.h>
#include <gf2/core/Vec2.h>

#include "BufferReference.h"
#include "Descriptor.h"
#include "GraphicsApi.h"
#include "RenderTarget.h"
#include "ShaderData.h"
#include "TextureReference.h"
#include "Vulkan.h"

namespace gf {
  class Buffer;
  class Pipeline;

  class GF_GRAPHICS_API RenderCommandBuffer {
  public:

    void set_viewport(RectF viewport) const;
    void set_scissor(RectI scissor) const;

    void bind_pipeline(const Pipeline* pipeline) const;
    void bind_vertex_buffer(const Buffer* buffer, std::size_t offset = 0) const;
    void bind_index_buffer(const Buffer* buffer, std::size_t offset = 0) const;

    void bind_descriptor(const Pipeline* pipeline, Descriptor descriptor) const;

    void push_constant(const Pipeline* pipeline, ShaderStage stage, std::size_t size, const void* data) const;

    template<typename T>
    void push_constant(const Pipeline* pipeline, ShaderStage stage, const T* data) const
    {
      if constexpr (std::is_same_v<T, ShaderDataType<T>>) {
        push_constant(pipeline, stage, sizeof(T), data);
      } else {
        const ShaderDataType<T> shader_data(*data);
        push_constant(pipeline, stage, sizeof(shader_data), &shader_data);
      }
    }

    void draw(std::size_t vertex_count) const;
    void draw_indexed(std::size_t index_count) const;

  private:
    friend class CommandBuffer;

    RenderCommandBuffer(VkCommandBuffer buffer)
    : m_command_buffer(buffer)
    {
    }

    VkCommandBuffer m_command_buffer = VK_NULL_HANDLE; // non-owning
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
    friend class BasicRenderer;
    friend class CommandBuffer;

    MemoryCommandBuffer(VkCommandBuffer buffer)
    : m_command_buffer(buffer)
    {
    }

    VkCommandBuffer m_command_buffer = VK_NULL_HANDLE; // non-owning
  };

  class GF_GRAPHICS_API CommandBuffer {
  public:

    RenderCommandBuffer begin_rendering(RenderTarget target, Color clear_color = Black) const;
    void end_rendering(RenderCommandBuffer buffer) const;

    MemoryCommandBuffer begin_memory() const;
    void end_memory(MemoryCommandBuffer buffer) const;

  private:
    friend class BasicRenderer;

    CommandBuffer(VkCommandBuffer buffer)
    : m_command_buffer(buffer)
    {
    }

    VkCommandBuffer m_command_buffer = VK_NULL_HANDLE; // non-owning
  };

}

#endif // GF_COMMAND_BUFFER_H
