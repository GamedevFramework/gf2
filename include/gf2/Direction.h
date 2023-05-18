// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_DIRECTION_H
#define GF_DIRECTION_H

#include <cstdint>

#include "CoreApi.h"
#include "Vec2.h"

namespace gf {

  enum class Direction : int8_t {
    Center = -1,
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3,
  };

  GF_CORE_API Vec2F unit(Direction direction);
  GF_CORE_API Vec2I displacement(Direction direction);
  GF_CORE_API float angle(Direction direction);
  GF_CORE_API Direction opposite(Direction direction);

} // namespace gf

#endif // GF_DIRECTION_H
