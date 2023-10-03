// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_VERTEX_INPUT_H
#define GF_VERTEX_INPUT_H

#include <cstdint>

#include <vector>

#include "Format.h"
#include "GraphicsApi.h"
#include "Vulkan.h"

namespace gf {

  struct GF_GRAPHICS_API VertexBinding {
    uint32_t binding = 0;
    std::size_t stride = 0;
  };

  struct GF_GRAPHICS_API VertexAttribute {
    uint32_t location = 0;
    uint32_t binding = 0;
    Format format = Format::Vec2F;
    std::size_t offset = 0;
  };

  struct GF_GRAPHICS_API VertexInput {
    std::vector<VertexBinding> bindings;
    std::vector<VertexAttribute> attributes;
  };

}

#endif // GF_VERTEX_INPUT_H
