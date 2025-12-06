// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GPU_GRAPHICS_PIPELINE_H
#define GF_GPU_GRAPHICS_PIPELINE_H

#include <string>

#include <gf2/core/Span.h>

#include "GpuShader.h"
#include "GraphicsApi.h"
#include "GraphicsHandle.h"
#include "GpuStates.h"
#include "VertexInput.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API GpuGraphicsPipeline {
  public:
    GpuGraphicsPipeline() = default;

    void set_debug_name(const std::string& name) const;

  private:
    friend class GpuGraphicsPipelineBuilder;
    friend class RenderPass;
    friend class RenderManager;

    details::GraphicsHandle<SDL_GPUGraphicsPipeline, SDL_ReleaseGPUGraphicsPipeline> m_handle;
  };

  class GF_GRAPHICS_API GpuGraphicsPipelineBuilder {
  public:
    GpuGraphicsPipelineBuilder() = default;

    GpuGraphicsPipelineBuilder& add_shader(GpuShaderStage stage, Span<const uint32_t> bytecode)
    {
      m_shaders.push_back({ stage, bytecode });
      return *this;
    }

    GpuGraphicsPipelineBuilder& set_primitive_topology(GpuPrimitiveTopology primitive_topology)
    {
      m_primitive_topology = primitive_topology;
      return *this;
    }

    GpuGraphicsPipelineBuilder& set_blend(GpuBlend blend)
    {
      m_blend = blend;
      return *this;
    }

    GpuGraphicsPipelineBuilder& set_vertex_input(VertexInput vertex_input)
    {
      m_vertex_input = std::move(vertex_input);
      return *this;
    }

    GpuGraphicsPipeline build(RenderManager* render_manager);

  private:
    friend class GpuGraphicsPipeline;

    struct Shader {
      GpuShaderStage stage = GpuShaderStage::Vertex;
      Span<const uint32_t> bytecode;
    };

    GpuPrimitiveTopology m_primitive_topology = GpuPrimitiveTopology::TriangleList;
    GpuBlend m_blend = AlphaBlending;
    VertexInput m_vertex_input;
    std::vector<Shader> m_shaders;
  };

}

#endif // GF_GPU_GRAPHICS_PIPELINE_H
