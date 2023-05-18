// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_ANCHOR_H
#define GF_ANCHOR_H

#include <cstdint>

namespace gf {

  enum class Anchor : uint8_t {
    TopLeft,
    TopCenter,
    TopRight,
    CenterLeft,
    Center,
    CenterRight,
    BottomLeft,
    BottomCenter,
    BottomRight,
  };

}

#endif // GF_ANCHOR_H
