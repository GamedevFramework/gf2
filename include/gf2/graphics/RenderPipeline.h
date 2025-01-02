// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_RENDER_PIPELINE_H
#define GF_RENDER_PIPELINE_H

#include <string>

#include <vulkan/vulkan.h>

#include <gf2/core/Span.h>

#include "Descriptor.h"
#include "GraphicsApi.h"
#include "RenderStates.h"
#include "ShaderStage.h"
#include "VertexInput.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API RenderPipelineLayout {
  public:
    RenderPipelineLayout() = default;
    RenderPipelineLayout(const RenderPipelineLayout&) = delete;
    RenderPipelineLayout(RenderPipelineLayout&& other) noexcept;
    ~RenderPipelineLayout();

    RenderPipelineLayout& operator=(const RenderPipelineLayout&) = delete;
    RenderPipelineLayout& operator=(RenderPipelineLayout&& other) noexcept;

    void set_debug_name(const std::string& name) const;

  private:
    friend class RenderPipelineBuilder;
    friend class RenderPipelineLayoutBuilder;
    friend class RenderCommandBuffer;

    RenderPipelineLayout(VkDevice device, VkPipelineLayout pipeline_layout)
    : m_device(device)
    , m_pipeline_layout(pipeline_layout)
    {
    }

    VkDevice m_device = VK_NULL_HANDLE; // non-owning
    VkPipelineLayout m_pipeline_layout = VK_NULL_HANDLE;
  };

  class GF_GRAPHICS_API RenderPipelineLayoutBuilder {
  public:

    RenderPipelineLayoutBuilder& add_descriptor_layout(DescriptorLayout* descriptor_layout)
    {
      m_descriptor_layouts.push_back(descriptor_layout);
      return *this;
    }

    RenderPipelineLayoutBuilder& set_push_constant_parameters(ShaderStage stage, std::size_t size)
    {
      m_push_constant.stage = stage;
      m_push_constant.size = size;
      return *this;
    }

    RenderPipelineLayout build(RenderManager* render_manager);

  private:
    std::vector<DescriptorLayout*> m_descriptor_layouts;
    struct {
      ShaderStage stage = ShaderStage::Vertex;
      std::size_t size = 0;
    } m_push_constant;
  };

  class GF_GRAPHICS_API RenderPipeline {
  public:
    RenderPipeline() = default;
    RenderPipeline(const RenderPipeline&) = delete;
    RenderPipeline(RenderPipeline&& other) noexcept;
    ~RenderPipeline();

    RenderPipeline& operator=(const RenderPipeline&) = delete;
    RenderPipeline& operator=(RenderPipeline&& other) noexcept;

    void set_debug_name(const std::string& name) const;

  private:
    friend class RenderPipelineBuilder;
    friend class RenderCommandBuffer;
    friend class RenderManager;

    RenderPipeline(VkDevice device, VkPipeline pipeline)
    : m_device(device)
    , m_pipeline(pipeline)
    {
    }

    VkDevice m_device = VK_NULL_HANDLE; // non-owning
    VkPipeline m_pipeline = VK_NULL_HANDLE;
  };

  class GF_GRAPHICS_API RenderPipelineBuilder {
  public:
    RenderPipelineBuilder() = default;

    RenderPipelineBuilder& add_shader(ShaderStage stage, Span<const uint32_t> bytecode)
    {
      m_shaders.push_back({ stage, bytecode });
      return *this;
    }

    RenderPipelineBuilder& set_primitive_topology(PrimitiveTopology primitive_topology)
    {
      m_primitive_topology = primitive_topology;
      return *this;
    }

    RenderPipelineBuilder& set_blend(Blend blend)
    {
      m_blend = blend;
      return *this;
    }

    RenderPipelineBuilder& set_vertex_input(VertexInput vertex_input)
    {
      m_vertex_input = std::move(vertex_input);
      return *this;
    }

    RenderPipelineBuilder& set_pipeline_layout(RenderPipelineLayout* pipeline_layout)
    {
      m_pipeline_layout = pipeline_layout;
      return *this;
    }

    RenderPipeline build(RenderManager* render_manager);

  private:
    friend class RenderPipeline;

    struct Shader {
      ShaderStage stage = ShaderStage::Vertex;
      Span<const uint32_t> bytecode;
    };

    PrimitiveTopology m_primitive_topology = PrimitiveTopology::TriangleList;
    Blend m_blend = AlphaBlending;
    VertexInput m_vertex_input;
    std::vector<Shader> m_shaders;
    RenderPipelineLayout* m_pipeline_layout = nullptr;
  };

}

#endif // GF_RENDER_PIPELINE_H
