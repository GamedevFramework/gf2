// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PIPELINE_H
#define GF_PIPELINE_H

#include <string>
#include <type_traits>

#include "Descriptor.h"
#include "GraphicsApi.h"
#include "RenderStates.h"
#include "Shader.h"
#include "VertexInput.h"
#include "Vulkan.h"

namespace gf {
  class Renderer;

  class GF_GRAPHICS_API Pipeline {
  public:
    Pipeline() = default;
    Pipeline(const Pipeline&) = delete;
    Pipeline(Pipeline&& other) noexcept;
    ~Pipeline();

    Pipeline& operator=(const Pipeline&) = delete;
    Pipeline& operator=(Pipeline&& other) noexcept;

    void set_debug_name(const std::string& name) const;

  private:
    friend class PipelineBuilder;
    friend class RenderCommandBuffer;
    friend class BasicRenderer;

    Pipeline(VkDevice device, VkPipelineLayout pipeline_layout, VkPipeline pipeline)
    : m_device(device)
    , m_pipeline_layout(pipeline_layout)
    , m_pipeline(pipeline)
    {
    }

    VkDevice m_device = VK_NULL_HANDLE; // non-owning
    VkPipelineLayout m_pipeline_layout = VK_NULL_HANDLE;
    VkPipeline m_pipeline = VK_NULL_HANDLE;
  };

  class GF_GRAPHICS_API PipelineBuilder {
  public:
    PipelineBuilder() = default;

    PipelineBuilder& add_shader(Shader* shader)
    {
      m_shaders.push_back(shader);
      return *this;
    }

    PipelineBuilder& set_primitive_topology(PrimitiveTopology primitive_topology)
    {
      m_primitive_topology = primitive_topology;
      return *this;
    }

    PipelineBuilder& set_blend(Blend blend)
    {
      m_blend = blend;
      return *this;
    }

    PipelineBuilder& set_vertex_input(VertexInput vertex_input)
    {
      m_vertex_input = std::move(vertex_input);
      return *this;
    }

    PipelineBuilder& add_descriptor_layout(DescriptorLayout* descriptor_layout)
    {
      m_descriptor_layouts.push_back(descriptor_layout);
      return *this;
    }

    PipelineBuilder& set_push_constant_parameters(ShaderStage stage, std::size_t size)
    {
      m_push_constant.stage = stage;
      m_push_constant.size = size;
      return *this;
    }

    Pipeline build(Renderer* renderer);

  private:
    friend class Pipeline;

    PrimitiveTopology m_primitive_topology = PrimitiveTopology::TriangleList;
    Blend m_blend = AlphaBlending;
    VertexInput m_vertex_input;
    std::vector<Shader*> m_shaders;
    std::vector<DescriptorLayout*> m_descriptor_layouts;
    struct {
      ShaderStage stage = ShaderStage::Vertex;
      std::size_t size = 0;
    } m_push_constant;
  };

}

#endif // GF_PIPELINE_H
