// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_FORMAT_H
#define GF_FORMAT_H

#include <limits>
#include <type_traits>

#include <SDL3/SDL_gpu.h>

namespace gf {

  // NOLINTNEXTLINE(performance-enum-size)
  enum class Format : std::underlying_type_t<SDL_GPUTextureFormat> {
    Undefined = SDL_GPU_TEXTUREFORMAT_INVALID,
    R8G8B8A8_SNorm = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_SNORM,
    R8G8B8A8_UNorm = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
    R32G32B32A32_Float = SDL_GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT,
    R8_UNorm = SDL_GPU_TEXTUREFORMAT_R8_UNORM,
    R32G32_Float = SDL_GPU_TEXTUREFORMAT_R32G32_FLOAT,
    RenderManager = std::numeric_limits<std::underlying_type_t<SDL_GPUTextureFormat>>::max(),
  };

}

#endif // GF_FORMAT_H
