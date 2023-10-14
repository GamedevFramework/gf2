// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_POLYGON_H
#define GF_POLYGON_H

#include <cstdint>

#include "CoreApi.h"
#include "Span.h"
#include "Vec2.h"

namespace gf {

  enum class Winding : uint8_t {
    None,
    Clockwise,
    Counterclockwise,
  };

  GF_CORE_API bool is_convex(Span<const Vec2F> polygon);

  GF_CORE_API float signed_area(Span<const Vec2F> polygon);
  GF_CORE_API float area(Span<const Vec2F> polygon);

  GF_CORE_API Winding winding(Span<const Vec2F> polygon);
  GF_CORE_API int winding_number(Vec2F point, Span<const Vec2F> polygon);

  GF_CORE_API bool contains(Span<const Vec2F> polygon, Vec2F point);

}

#endif // GF_POLYGON_H
