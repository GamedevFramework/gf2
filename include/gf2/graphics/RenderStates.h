// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_RENDER_STATES_H
#define GF_RENDER_STATES_H

#include <type_traits>

#include <vulkan/vulkan.h>

#include "GraphicsApi.h"

namespace gf {

  // NOLINTNEXTLINE(performance-enum-size)
  enum class PrimitiveTopology : std::underlying_type_t<VkPrimitiveTopology> {
    PointList = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
    LineList = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
    LineStrip = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
    TriangleList = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    TriangleStrip = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
    TriangleFan = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN,
    LineListWithAdjacency = VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY,
    LineStripWithAdjacency = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY,
    TriangleListWithAdjacency = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY,
    TriangleStripWithAdjacency = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY,
    PatchList = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST,
  };

  // NOLINTNEXTLINE(performance-enum-size)
  enum class BlendFactor : std::underlying_type_t<VkBlendFactor> {
    Zero = VK_BLEND_FACTOR_ZERO,
    One = VK_BLEND_FACTOR_ONE,
    SrcColor = VK_BLEND_FACTOR_SRC_COLOR,
    OneMinusSrcColor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
    DstColor = VK_BLEND_FACTOR_DST_COLOR,
    OneMinusDstColor = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
    SrcAlpha = VK_BLEND_FACTOR_SRC_ALPHA,
    OneMinusSrcAlpha = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    DstAlpha = VK_BLEND_FACTOR_DST_ALPHA,
    OneMinusDstAlpha = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
  // NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#if 0
    ConstantColor = VK_BLEND_FACTOR_CONSTANT_COLOR,
    OneMinusConstantColor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
    ConstantAlpha = VK_BLEND_FACTOR_CONSTANT_ALPHA,
    OneMinusConstantAlpha = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
    SrcAlphaSaturate = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
#endif
  };

  // NOLINTNEXTLINE(performance-enum-size)
  enum class BlendOp : std::underlying_type_t<VkBlendOp> {
    Add = VK_BLEND_OP_ADD,
    Substract = VK_BLEND_OP_SUBTRACT,
    ReverseSubstract = VK_BLEND_OP_REVERSE_SUBTRACT,
    Min = VK_BLEND_OP_MIN,
    Max = VK_BLEND_OP_MAX,
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
