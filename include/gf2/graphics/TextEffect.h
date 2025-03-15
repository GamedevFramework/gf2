// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_TEXT_EFFECT_H
#define GF_TEXT_EFFECT_H

#include <gf2/core/Color.h>

#include "GraphicsApi.h"

namespace gf {

  struct GF_GRAPHICS_API TextEffect {
    Color outline_color = Transparent;
    float outline_thickness = 0.0f;
  };

}

#endif // GF_TEXT_EFFECT_H
