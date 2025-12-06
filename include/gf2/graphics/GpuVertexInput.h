// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GPU_VERTEX_INPUT_H
#define GF_GPU_VERTEX_INPUT_H

#include <cstdint>

#include <vector>

#include <SDL3/SDL_gpu.h>

#include "GraphicsApi.h"

namespace gf {

  struct GF_GRAPHICS_API GpuVertexBufferDescription {
    uint32_t slot = 0;
    std::size_t pitch = 0;
  };

  // NOLINTNEXTLINE(performance-enum-size)
  enum class GpuVertexElementFormat : std::underlying_type_t<SDL_GPUVertexElementFormat> {
    Invalid = SDL_GPU_VERTEXELEMENTFORMAT_INVALID,
    Int1 = SDL_GPU_VERTEXELEMENTFORMAT_INT,
    Int2 = SDL_GPU_VERTEXELEMENTFORMAT_INT2,
    Int3 = SDL_GPU_VERTEXELEMENTFORMAT_INT3,
    Int4 = SDL_GPU_VERTEXELEMENTFORMAT_INT4,
    UInt1 = SDL_GPU_VERTEXELEMENTFORMAT_UINT,
    UInt2 = SDL_GPU_VERTEXELEMENTFORMAT_UINT2,
    UInt3 = SDL_GPU_VERTEXELEMENTFORMAT_UINT3,
    UInt4 = SDL_GPU_VERTEXELEMENTFORMAT_UINT4,
    Float1 = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT,
    Float2 = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
    Float3 = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
    Float4 = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
    Byte2 = SDL_GPU_VERTEXELEMENTFORMAT_BYTE2,
    Byte4 = SDL_GPU_VERTEXELEMENTFORMAT_BYTE4,
    UByte2 = SDL_GPU_VERTEXELEMENTFORMAT_UBYTE2,
    UByte4 = SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4,
    Byte2_Norm = SDL_GPU_VERTEXELEMENTFORMAT_BYTE2_NORM,
    Byte4_Norm = SDL_GPU_VERTEXELEMENTFORMAT_BYTE4_NORM,
    UByte2_Norm = SDL_GPU_VERTEXELEMENTFORMAT_UBYTE2_NORM,
    UByte4_Norm = SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM,
  };

  struct GF_GRAPHICS_API GpuVertexAttribute {
    uint32_t location = 0;
    uint32_t slot = 0;
    GpuVertexElementFormat format = GpuVertexElementFormat::Float2;
    std::size_t offset = 0;
  };

  struct GF_GRAPHICS_API GpuVertexInput {
    std::vector<GpuVertexBufferDescription> buffer_descriptions;
    std::vector<GpuVertexAttribute> attributes;
  };

}

#endif // GF_GPU_VERTEX_INPUT_H
