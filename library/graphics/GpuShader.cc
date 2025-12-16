// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/GpuShader.h>

#include <gf2/core/Log.h>

#include <gf2/graphics/GpuDevice.h>

namespace gf {

  GpuShader::GpuShader(GpuShaderStage stage, gf::Span<const uint8_t> binary, const GpuShaderInput& input, GpuDevice* device, const std::string& name)
  {
    SDL_GPUShaderCreateInfo info = {};
    info.code_size = binary.size();
    info.code = binary.data();
    info.entrypoint = "main";
    info.format = SDL_GPU_SHADERFORMAT_SPIRV;
    info.stage = static_cast<SDL_GPUShaderStage>(stage);
    info.num_samplers = static_cast<Uint32>(input.samplers);
    info.num_storage_textures = static_cast<Uint32>(input.storage_textures);
    info.num_storage_buffers = static_cast<Uint32>(input.storage_buffers);
    info.num_uniform_buffers = static_cast<Uint32>(input.uniform_buffers);

    if (!name.empty()) {
      info.props = SDL_CreateProperties();
      SDL_SetStringProperty(info.props, SDL_PROP_GPU_SHADER_CREATE_NAME_STRING, name.c_str());
    }

    SDL_GPUShader* shader = SDL_CreateGPUShader(*device, &info);

    if (!name.empty()) {
      SDL_DestroyProperties(info.props);
    }

    if (shader == nullptr) {
      Log::fatal("Could not create shader: {}", SDL_GetError());
    }

    m_handle = { *device, shader };
  }


}
