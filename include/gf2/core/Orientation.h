// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_ORIENTATION_H
#define GF_ORIENTATION_H

#include <cstdint>

#include "CoreApi.h"
#include "Vec2.h"

namespace gf {

  enum class Orientation : int8_t {
    Center = -1,
    North = 0,
    NorthEast = 1,
    East = 2,
    SouthEast = 3,
    South = 4,
    SouthWest = 5,
    West = 6,
    NorthWest = 7,
  };

  GF_CORE_API Vec2F unit(Orientation orientation);
  GF_CORE_API Vec2I displacement(Orientation orientation);
  GF_CORE_API float angle(Orientation orientation);
  GF_CORE_API Orientation orientation(float angle);
  GF_CORE_API Orientation opposite(Orientation orientation);

} // namespace gf

#endif // GF_ORIENTATION_H
