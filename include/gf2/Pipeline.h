// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PIPELINE_H
#define GF_PIPELINE_H

#include <type_traits>

#include <vulkan/vulkan.h>

#include "GraphicsApi.h"
#include "RenderStates.h"
#include "Shader.h"
#include "VertexInput.h"

namespace gf {

  class GF_GRAPHICS_API Pipeline {
  public:
    Pipeline(const Pipeline&) = delete;
    Pipeline(Pipeline&& other) noexcept;
    ~Pipeline();

    Pipeline& operator=(const Pipeline&) = delete;
    Pipeline& operator=(Pipeline&& other) noexcept;

  private:
    friend class PipelineBuilder;
    friend class CommandBuffer;

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

    PipelineBuilder& set_pritimive_topology(PrimitiveTopology primitive_topology)
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

    Pipeline build(Renderer* renderer);

  private:
    friend class Pipeline;

    PrimitiveTopology m_primitive_topology = PrimitiveTopology::TriangleList;
    Blend m_blend = AlphaBlending;
    VertexInput m_vertex_input;
    std::vector<Shader*> m_shaders;
  };

}

#endif // GF_PIPELINE_H
