// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_RENDER_OBJECT_H
#define GF_RENDER_OBJECT_H

#include <cstdint>

#include <gf2/core/Mat3.h>

#include "GraphicsApi.h"

namespace gf {
  class Buffer;
  class Texture;

  enum class RenderPipelineType : uint8_t {
    Default,
    Text,
    Imgui,
  };

  struct GF_GRAPHICS_API RenderGeometry {
    RenderPipelineType pipeline = RenderPipelineType::Default;
    const Buffer* vertices = nullptr;
    const Buffer* indices = nullptr;
    std::size_t count = 0;
    std::size_t first = 0;
    std::ptrdiff_t offset = 0;
    const Texture* texture = nullptr;
  };

  struct GF_GRAPHICS_API RenderObject {
    int32_t priority = 0;
    RenderGeometry geometry;
    Mat3F transform = Identity3F;
  };

}

#endif // GF_RENDER_OBJECT_H
