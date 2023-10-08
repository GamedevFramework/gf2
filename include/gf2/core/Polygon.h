// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_POLYGON_H
#define GF_POLYGON_H

#include <cstdint>

#include "Span.h"
#include "Vec2.h"

namespace gf {

  enum class Winding : uint8_t {
    None,
    Clockwise,
    Counterclockwise,
  };

  bool is_convex(Span<const Vec2F> polygon);

  float signed_area(Span<const Vec2F> polygon);
  float area(Span<const Vec2F> polygon);

  Winding winding(Span<const Vec2F> polygon);
  int winding_number(Vec2F point, Span<const Vec2F> polygon);

  bool contains(Span<const Vec2F> polygon, Vec2F point);

}

#endif // GF_POLYGON_H
