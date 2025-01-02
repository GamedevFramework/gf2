// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Polygon.h>

namespace gf {

  bool is_convex(Span<const Vec2F> polygon)
  {
    const std::size_t size = polygon.size();

    if (size <= 3) {
      return true;
    }

    int current_sign = 0;

    for (std::size_t i = 0; i < size; ++i) {
      const float x = gf::cross(polygon[(i + 1) % size] - polygon[i], polygon[(i + 2) % size] - polygon[(i + 1) % size]);

      if (std::abs(x) > Epsilon) {
        const int sign = gf::sign(x);

        if (current_sign != 0 && sign != current_sign) {
          return false;
        }

        current_sign = sign;
      }
    }

    return true;
  }

  // https://en.wikipedia.org/wiki/Shoelace_formula
  // TODO: use Kahan summation?
  float signed_area(Span<const Vec2F> polygon)
  {
    const std::size_t size = polygon.size();
    assert(size > 2);
    float area = 0.0f;

    for (std::size_t i = 0; i < size - 1; ++i) {
      area += gf::cross(polygon[i], polygon[i + 1]);
    }

    area += gf::cross(polygon[size - 1], polygon[0]);
    return area;
  }

  float area(Span<const Vec2F> polygon)
  {
    return 0.5f * std::abs(signed_area(polygon));
  }

  Winding winding(Span<const Vec2F> polygon)
  {
    const float area = signed_area(polygon);

    if (area > 0.0f) {
      return Winding::Clockwise;
    }

    if (area < 0.0f) {
      return Winding::Counterclockwise;
    }

    return Winding::None;
  }

  int winding_number(Vec2F point, Span<const Vec2F> polygon)
  {
    const std::size_t size = polygon.size();
    int wn = 0;

    for (std::size_t i = 0; i < size; ++i) {
      const Vec2F curr = polygon[i];
      const Vec2F next = polygon[(i + 1) % size];

      if (curr.y <= point.y) {
        if (next.y > point.y) {
          if (gf::cross(next - curr, point - curr) > 0) {
            ++wn;
          }
        }
      } else {
        if (next.y < point.y) {
          if (gf::cross(next - curr, point - curr) < 0) {
            --wn;
          }
        }
      }
    }

    return wn;
  }

  bool contains(Span<const Vec2F> polygon, Vec2F point)
  {
    return winding_number(point, polygon) != 0;
  }

}
