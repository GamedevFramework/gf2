// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GPU_STATES_H
#define GF_GPU_STATES_H

#include <type_traits>

#include <SDL3/SDL_gpu.h>

#include "GraphicsApi.h"

namespace gf {

  // NOLINTNEXTLINE(performance-enum-size)
  enum class GpuPrimitiveTopology : std::underlying_type_t<SDL_GPUPrimitiveType> {
    TriangleList = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
    TriangleStrip = SDL_GPU_PRIMITIVETYPE_TRIANGLESTRIP,
    LineList = SDL_GPU_PRIMITIVETYPE_LINELIST,
    LineStrip = SDL_GPU_PRIMITIVETYPE_LINESTRIP,
  };

  // NOLINTNEXTLINE(performance-enum-size)
  enum class GpuBlendFactor : std::underlying_type_t<SDL_GPUBlendFactor> {
    Invalid = SDL_GPU_BLENDFACTOR_INVALID,
    Zero = SDL_GPU_BLENDFACTOR_ZERO,
    One = SDL_GPU_BLENDFACTOR_ONE,
    SrcColor = SDL_GPU_BLENDFACTOR_SRC_COLOR,
    OneMinusSrcColor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_COLOR,
    DstColor = SDL_GPU_BLENDFACTOR_DST_COLOR,
    OneMinusDstColor = SDL_GPU_BLENDFACTOR_ONE_MINUS_DST_COLOR,
    SrcAlpha = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
    OneMinusSrcAlpha = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
    DstAlpha = SDL_GPU_BLENDFACTOR_DST_ALPHA,
    OneMinusDstAlpha = SDL_GPU_BLENDFACTOR_ONE_MINUS_DST_ALPHA,
  };

  // NOLINTNEXTLINE(performance-enum-size)
  enum class GpuBlendOp : std::underlying_type_t<SDL_GPUBlendOp> {
    Invalid = SDL_GPU_BLENDOP_INVALID,
    Add = SDL_GPU_BLENDOP_ADD,
    Substract = SDL_GPU_BLENDOP_SUBTRACT,
    ReverseSubstract = SDL_GPU_BLENDOP_REVERSE_SUBTRACT,
    Min = SDL_GPU_BLENDOP_MIN,
    Max = SDL_GPU_BLENDOP_MAX,
  };

  struct GF_GRAPHICS_API GpuBlend {
    GpuBlendFactor color_src;
    GpuBlendFactor color_dst;
    GpuBlendOp color_op;
    GpuBlendFactor alpha_src;
    GpuBlendFactor alpha_dst;
    GpuBlendOp alpha_op;
  };

  inline constexpr GpuBlend AlphaBlending = {
    GpuBlendFactor::SrcAlpha,
    GpuBlendFactor::OneMinusSrcAlpha,
    GpuBlendOp::Add,
    GpuBlendFactor::One,
    GpuBlendFactor::OneMinusSrcAlpha,
    GpuBlendOp::Add
  };

}

#endif // GF_GPU_STATES_H
