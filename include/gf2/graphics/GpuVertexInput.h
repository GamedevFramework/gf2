// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GPU_VERTEX_INPUT_H
#define GF_GPU_VERTEX_INPUT_H

#include <cstdint>

#include <vector>

#include "Format.h"
#include "GraphicsApi.h"

namespace gf {

  struct GF_GRAPHICS_API GpuVertexBinding {
    uint32_t binding = 0;
    std::size_t stride = 0;
  };

  struct GF_GRAPHICS_API GpuVertexAttribute {
    uint32_t location = 0;
    uint32_t binding = 0;
    Format format = Format::R32G32_Float;
    std::size_t offset = 0;
  };

  struct GF_GRAPHICS_API GpuVertexInput {
    std::vector<GpuVertexBinding> bindings;
    std::vector<GpuVertexAttribute> attributes;
  };

}

#endif // GF_GPU_VERTEX_INPUT_H
