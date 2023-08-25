// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PIPELINE_H
#define GF_PIPELINE_H

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

  private:
    friend class PipelineBuilder;
    friend class CommandBuffer;
    friend class BasicRenderer;

    Pipeline(VkDevice device, VkPipelineLayout pipeline_layout, VkPipeline pipeline, VkDescriptorSetLayout descriptors_layout)
    : m_device(device)
    , m_pipeline_layout(pipeline_layout)
    , m_pipeline(pipeline)
    , m_descriptors_layout(descriptors_layout)
    {
    }

    VkDevice m_device = VK_NULL_HANDLE; // non-owning
    VkPipelineLayout m_pipeline_layout = VK_NULL_HANDLE;
    VkPipeline m_pipeline = VK_NULL_HANDLE;
    VkDescriptorSetLayout m_descriptors_layout = VK_NULL_HANDLE;
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

    PipelineBuilder& add_descriptor_binding(DescriptorBinding descriptor_binding)
    {
      m_descriptor_bindings.push_back(descriptor_binding);
      return *this;
    }

    Pipeline build(Renderer* renderer);

  private:
    friend class Pipeline;

    PrimitiveTopology m_primitive_topology = PrimitiveTopology::TriangleList;
    Blend m_blend = AlphaBlending;
    VertexInput m_vertex_input;
    std::vector<Shader*> m_shaders;
    std::vector<DescriptorBinding> m_descriptor_bindings;
  };

}

#endif // GF_PIPELINE_H
