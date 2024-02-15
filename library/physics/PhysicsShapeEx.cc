// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/physics/PhysicsShapeEx.h>

#include <cassert>

namespace gf {

  std::vector<PhysicsShape> make_polyline_shapes(PhysicsBody* body, Span<const Vec2F> polyline, float radius, PolylineType type)
  {
    const PolylineView polyline_view = { type, polyline };
    const std::size_t count = polyline.size() - 1;

    std::vector<PhysicsShape> shapes;

    for (std::size_t i = 0; i < count; ++i) {
      const Vec2F a = polyline[i];
      const Vec2F b = polyline[i + 1];

      const Vec2F prev = polyline_view.has_prev(i) ? polyline_view.prev_point(i) : a;
      const Vec2F next = polyline_view.has_next(i + 1) ? polyline_view.next_point(i + 1) : b;

      shapes.emplace_back(PhysicsShape::make_segment(body, a, b, radius, prev, next));
    }

    if (type == PolylineType::Loop) {
      const Vec2F a = polyline.back();
      const Vec2F b = polyline.front();

      assert(polyline_view.has_prev(count));
      const Vec2F prev = polyline_view.prev_point(count);
      assert(polyline_view.has_next(0));
      const Vec2F next = polyline_view.next_point(0);

      shapes.emplace_back(PhysicsShape::make_segment(body, a, b, radius, prev, next));
    }

    return shapes;
  }

  std::vector<PhysicsShape> make_polyline_shapes(PhysicsBody* body, const Polyline& polyline, float radius)
  {
    return make_polyline_shapes(body, polyline.points, radius, polyline.type);
  }

}
