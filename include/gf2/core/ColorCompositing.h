// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_COLOR_COMPOSITING_H
#define GF_COLOR_COMPOSITING_H

#include <cstdint>

#include "Color.h"
#include "CoreApi.h"

namespace gf {

  enum class BlendMode : uint8_t {
    Normal,
    Darken,
    Multiply,
    ColorBurn,
    LinearBurn,
    Lighten,
    Screen,
    ColorDodge,
    LinearDodge, // a.k.a. Add
    Overlay,
    SoftLight,
    HardLight,
    VividLight,
    LinearLight,
    HardMix,
    Difference,
    Exclusion,
    Substract,
    Divide,
  };

  enum class CompositingOperation : uint8_t {
    Clear,
    Source,
    Destination,
    SourceOver,
    DestinationOver,
    SourceIn,
    DestinationIn,
    SourceOut,
    DestinationOut,
    SourceAtop,
    DestinationAtop,
    Xor,
  };

  GF_CORE_API Color mix_colors(Color source, Color backdrop, BlendMode mode = BlendMode::Normal);
  GF_CORE_API Color compose_colors(Color source, Color backdrop, CompositingOperation operation = CompositingOperation::Source);

  GF_CORE_API Color combine_colors(Color source, Color backdrop, BlendMode mode = BlendMode::Normal, CompositingOperation operation = CompositingOperation::Source);
  GF_CORE_API Color blend_colors(Color source, Color backdrop,  BlendMode mode = BlendMode::Normal);

}

#endif // GF_COLOR_COMPOSITING_H
