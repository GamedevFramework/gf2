// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/GpuShader.h>

#include <gf2/graphics/GpuDevice.h>

namespace gf {

  GpuShader::GpuShader(GpuShaderStage stage, gf::Span<const uint8_t> binary, const GpuShaderInput& input, GpuDevice* device)
  {
    SDL_GPUShaderCreateInfo info = {};
    info.code_size = binary.size();
    info.code = binary.data();
    info.entrypoint = "main";
    info.format = SDL_GPU_SHADERFORMAT_SPIRV;
    info.stage = static_cast<SDL_GPUShaderStage>(stage);
    info.num_samplers = static_cast<Uint32>(input.samplers);
    info.num_storage_textures = static_cast<Uint32>(input.storage_textures);
    info.num_storage_buffers = static_cast<Uint32>(input.storage_textures);
    info.num_uniform_buffers = static_cast<Uint32>(input.uniform_buffers);

    SDL_GPUShader* shader = SDL_CreateGPUShader(*device, &info);
    m_handle = { *device, shader };
  }


}
