// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GPU_GRAPHICS_PIPELINE_H
#define GF_GPU_GRAPHICS_PIPELINE_H

#include <string>

#include <SDL3/SDL_gpu.h>

#include <gf2/core/Span.h>

#include "GpuShader.h"
#include "GraphicsApi.h"
#include "GraphicsHandle.h"
#include "GpuStates.h"
#include "GpuVertexInput.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API GpuGraphicsPipeline {
  public:
    GpuGraphicsPipeline() = default;

  private:
    friend class GpuGraphicsPipelineBuilder;
    friend class RenderPass;
    friend class RenderManager;

    GpuGraphicsPipeline(SDL_GPUDevice* device, SDL_GPUGraphicsPipeline* pipeline)
    : m_handle(device, pipeline)
    {
    }

    details::GraphicsHandle<SDL_GPUGraphicsPipeline, SDL_ReleaseGPUGraphicsPipeline> m_handle;
  };

  class GF_GRAPHICS_API GpuGraphicsPipelineBuilder {
  public:
    GpuGraphicsPipelineBuilder() = default;

    GpuGraphicsPipelineBuilder& add_shader(GpuShaderStage stage, GpuShader* shader)
    {
      switch (stage) {
        case gf::GpuShaderStage::Vertex:
          m_vertex_shader = shader;
          break;
        case gf::GpuShaderStage::Fragment:
          m_fragment_shader = shader;
          break;
      }

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

    GpuGraphicsPipelineBuilder& set_vertex_input(GpuVertexInput vertex_input)
    {
      m_vertex_input = std::move(vertex_input);
      return *this;
    }

    GpuGraphicsPipeline build(RenderManager* render_manager);

  private:
    friend class GpuGraphicsPipeline;

    GpuShader* m_vertex_shader = nullptr;
    GpuShader* m_fragment_shader = nullptr;
    GpuVertexInput m_vertex_input;
    GpuPrimitiveTopology m_primitive_topology = GpuPrimitiveTopology::TriangleList;
    GpuBlend m_blend = AlphaBlending;
  };

}

#endif // GF_GPU_GRAPHICS_PIPELINE_H
