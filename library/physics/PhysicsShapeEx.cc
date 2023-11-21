// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/physics/PhysicsShapeEx.h>
// clang-format on

#include <cassert>

namespace gf {

  namespace {

    struct Polyline {
      Span<const Vec2F> polyline;
      PolylineType type = PolylineType::Chain;

      bool has_prev(std::size_t i) const
      {
        return i > 0 || type == PolylineType::Loop;
      }

      Vec2F prev_point(std::size_t i) const
      {
        assert(has_prev(i));

        if (i == 0) {
          return polyline.back();
        }

        return polyline[i - 1];
      }

      bool has_next(std::size_t i) const
      {
        return i < polyline.size() - 1 || type == PolylineType::Loop;
      }

      Vec2F next_point(std::size_t i) const
      {
        assert(has_next(i));

        if (i == polyline.size() - 1) {
          return polyline.front();
        }

        return polyline[i + 1];
      }

    };


  }

  std::vector<PhysicsShape> make_polyline_shapes(PhysicsBody* body, Span<const Vec2F> polyline, float radius, PolylineType type)
  {
    Polyline polyline_ex = { polyline, type };

    std::vector<PhysicsShape> shapes;

    for (std::size_t i = 0; i < polyline.size() - 1; ++i) {
      Vec2F a = polyline[i];
      Vec2F b = polyline[i + 1];

      Vec2F prev = polyline_ex.has_prev(i) ? polyline_ex.prev_point(i) : a;
      Vec2F next = polyline_ex.has_next(i + 1) ? polyline_ex.next_point(i + 1) : b;

      shapes.emplace_back(PhysicsShape::make_segment(body, a, b, radius, prev, next));
    }

    return shapes;
  }

}
