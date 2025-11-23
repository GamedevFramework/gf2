// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_RENDER_STATES_H
#define GF_RENDER_STATES_H

#include <type_traits>

#include "GraphicsApi.h"

namespace gf {

  // NOLINTNEXTLINE(performance-enum-size)
  enum class PrimitiveTopology  {
    PointList,
    LineList,
    LineStrip,
    TriangleList,
    TriangleStrip,
    TriangleFan,
    LineListWithAdjacency,
    LineStripWithAdjacency,
    TriangleListWithAdjacency,
    TriangleStripWithAdjacency,
    PatchList,
  };

  // NOLINTNEXTLINE(performance-enum-size)
  enum class BlendFactor {
    Zero,
    One,
    SrcColor,
    OneMinusSrcColor,
    DstColor,
    OneMinusDstColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    DstAlpha,
    OneMinusDstAlpha,
  };

  // NOLINTNEXTLINE(performance-enum-size)
  enum class BlendOp {
    Add,
    Substract,
    ReverseSubstract,
    Min,
    Max,
  };

  struct GF_GRAPHICS_API Blend {
    BlendFactor color_src;
    BlendFactor color_dst;
    BlendOp color_op;
    BlendFactor alpha_src;
    BlendFactor alpha_dst;
    BlendOp alpha_op;
  };

  inline constexpr Blend AlphaBlending = {
    BlendFactor::SrcAlpha,
    BlendFactor::OneMinusSrcAlpha,
    BlendOp::Add,
    BlendFactor::One,
    BlendFactor::OneMinusSrcAlpha,
    BlendOp::Add
  };

}

#endif // GF_RENDER_STATES_H
