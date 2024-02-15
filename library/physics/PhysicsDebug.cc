// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/physics/PhysicsDebug.h>

#include <gf2/physics/PhysicsShape.h>

namespace gf {

  void PhysicsDebug::draw_circle([[maybe_unused]] Vec2F center, [[maybe_unused]] float angle, [[maybe_unused]] float radius, [[maybe_unused]] Color outline_color, [[maybe_unused]] Color fill_color)
  {
  }

  void PhysicsDebug::draw_segment([[maybe_unused]] Vec2F a, [[maybe_unused]] Vec2F b, [[maybe_unused]] Color color)
  {
  }

  void PhysicsDebug::draw_fat_segment([[maybe_unused]] Vec2F a, [[maybe_unused]] Vec2F b, [[maybe_unused]] float radius, [[maybe_unused]] Color outline_color, [[maybe_unused]] Color fill_color)
  {
  }

  void PhysicsDebug::draw_polygon([[maybe_unused]] Span<const Vec2F> points, [[maybe_unused]] float radius, [[maybe_unused]] Color outline_color, [[maybe_unused]] Color fill_color)
  {
  }

  void PhysicsDebug::draw_dot([[maybe_unused]] float size, [[maybe_unused]] Vec2F location, [[maybe_unused]] Color color)
  {
  }

  Color PhysicsDebug::color_for_shape([[maybe_unused]] PhysicsShape shape) // NOLINT(performance-unnecessary-value-param)
  {
    return Gray;
  }

}
