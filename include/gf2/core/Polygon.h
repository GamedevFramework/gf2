// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_POLYGON_H
#define GF_POLYGON_H

#include <cstdint>

#include <span>

#include "CoreApi.h"
#include "Vec2.h"

namespace gf {

  enum class Winding : uint8_t {
    None,
    Clockwise,
    Counterclockwise,
  };

  GF_CORE_API bool is_convex(std::span<const Vec2F> polygon);

  GF_CORE_API float signed_area(std::span<const Vec2F> polygon);
  GF_CORE_API float area(std::span<const Vec2F> polygon);

  GF_CORE_API Winding winding(std::span<const Vec2F> polygon);
  GF_CORE_API int winding_number(Vec2F point, std::span<const Vec2F> polygon);

  GF_CORE_API bool contains(std::span<const Vec2F> polygon, Vec2F point);

}

#endif // GF_POLYGON_H
