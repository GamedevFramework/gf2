// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_MOVE_H
#define GF_MOVE_H

#include <cstdint>

#include "CoreApi.h"

namespace gf {

  enum class AngularMove : uint8_t {
    None,
    Left,
    Right,
  };

  GF_CORE_API float angular_factor(AngularMove move);

  enum class LinearMove : uint8_t {
    None,
    Backward,
    Forward,
  };

  GF_CORE_API float linear_factor(LinearMove move);

} // namespace gf

#endif // GF_MOVE_H
