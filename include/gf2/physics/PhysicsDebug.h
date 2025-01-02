// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_DEBUG_H
#define GF_PHYSICS_DEBUG_H

#include <type_traits>

#include <chipmunk/chipmunk.h>

#include <gf2/core/Color.h>
#include <gf2/core/Flags.h>
#include <gf2/core/Span.h>
#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"

namespace gf {
  class PhysicsShape;

  // NOLINTNEXTLINE(performance-enum-size)
  enum class PhysicsDebugFeature : std::underlying_type_t<cpSpaceDebugDrawFlags> {
    Shapes = CP_SPACE_DEBUG_DRAW_SHAPES,
    Constraints = CP_SPACE_DEBUG_DRAW_CONSTRAINTS,
    CollisionPoints = CP_SPACE_DEBUG_DRAW_COLLISION_POINTS,
  };

  template<>
  struct EnableBitmaskOperators<PhysicsDebugFeature> : std::true_type {
  };

  class GF_PHYSICS_API PhysicsDebug {
  public:
    PhysicsDebug() = default;
    PhysicsDebug(const PhysicsDebug&) = default;
    PhysicsDebug(PhysicsDebug&&) noexcept = default;
    virtual ~PhysicsDebug() = default;

    PhysicsDebug& operator=(const PhysicsDebug&) = default;
    PhysicsDebug& operator=(PhysicsDebug&&) noexcept = default;

    virtual void draw_circle(Vec2F center, float angle, float radius, Color outline_color, Color fill_color);
    virtual void draw_segment(Vec2F a, Vec2F b, Color color);
    virtual void draw_fat_segment(Vec2F a, Vec2F b, float radius, Color outline_color, Color fill_color);
    virtual void draw_polygon(Span<const Vec2F> points, float radius, Color outline_color, Color fill_color);
    virtual void draw_dot(float size, Vec2F location, Color color);
    virtual Color color_for_shape(PhysicsShape shape);
  };

}

#endif // GF_PHYSICS_DEBUG_H
