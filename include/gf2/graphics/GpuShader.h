// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GPU_SHADER_H
#define GF_GPU_SHADER_H

#include <string>
#include <type_traits>

#include <SDL3/SDL_gpu.h>

#include <gf2/core/Span.h>

#include "GraphicsApi.h"
#include "GraphicsHandle.h"

namespace gf {
  class GpuDevice;

  // NOLINTNEXTLINE(performance-enum-size)
  enum class GpuShaderStage : std::underlying_type_t<SDL_GPUShaderStage> {
    Vertex = SDL_GPU_SHADERSTAGE_VERTEX,
    Fragment = SDL_GPU_SHADERSTAGE_FRAGMENT,
  };

  struct GF_GRAPHICS_API GpuShaderInput {
    std::size_t samplers = 0;
    std::size_t storage_textures = 0;
    std::size_t storage_buffers = 0;
    std::size_t uniform_buffers = 0;
  };

  class GF_GRAPHICS_API GpuShader {
  public:
    GpuShader() = default;
    GpuShader(GpuShaderStage stage, gf::Span<const uint8_t> binary, const GpuShaderInput& input, GpuDevice* device, const std::string& name);

  private:
    friend class GpuDevice;
    friend class GpuGraphicsPipelineBuilder;

    GpuShader(SDL_GPUDevice* device, SDL_GPUShader* shader)
    : m_handle(device, shader)
    {
    }

    details::GraphicsHandle<SDL_GPUShader, SDL_ReleaseGPUShader> m_handle;
  };

}

#endif // GF_GPU_SHADER_H
