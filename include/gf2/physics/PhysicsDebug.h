// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_DEBUG_H
#define GF_PHYSICS_DEBUG_H

#include <cstdint>

#include <limits>
#include <string_view>

#include <gf2/core/Circ.h>
#include <gf2/core/Color.h>
#include <gf2/core/Flags.h>
#include <gf2/core/Geometry.h>
#include <gf2/core/Rect.h>
#include <gf2/core/Span.h>
#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"
#include "PhysicsTransform.h"

namespace gf {

  enum class PhysicsDebugFeature : uint16_t {
    DrawShapes          = 0x0001,
    DrawJoints          = 0x0002,
    DrawJointsExtra     = 0x0004,
    DrawBounds          = 0x0008,
    DrawMass            = 0x0010,
    DrawBodyNames       = 0x0020,
    DrawContactPoints   = 0x0040,
    DrawGraphColors     = 0x0080,
    DrawContactFeatures = 0x0100,
    DrawContactNormals  = 0x0200,
    DrawContactForces   = 0x0400,
    DrawFrictionForces  = 0x0800,
    DrawIslands         = 0x1000,
  };

  template<>
  struct EnableBitmaskOperators<PhysicsDebugFeature> : std::true_type {
  };

  namespace details {

    inline constexpr RectF PhysicsDebugInitialBounds = RectF::from_min_max(
      { std::numeric_limits<float>::lowest() / 2.0f, std::numeric_limits<float>::lowest() / 2.0f },
      { std::numeric_limits<float>::max() / 2.0f, std::numeric_limits<float>::max() / 2.0f }
    );

  }

  struct GF_PHYSICS_API PhysicsDebugOptions {
    RectF bounds = details::PhysicsDebugInitialBounds;
    float force_scale = 1.0f;
    float joint_scale = 1.0f;
    Flags<PhysicsDebugFeature> features = PhysicsDebugFeature::DrawShapes;
  };

  class GF_PHYSICS_API PhysicsDebug {
  public:
    PhysicsDebug() = default;
    PhysicsDebug(const PhysicsDebug&) = default;
    PhysicsDebug(PhysicsDebug&&) noexcept = default;
    virtual ~PhysicsDebug();

    PhysicsDebug& operator=(const PhysicsDebug&) = default;
    PhysicsDebug& operator=(PhysicsDebug&&) noexcept = default;

    virtual void draw_polygon(Span<const Vec2F> polygon, Color color);
    virtual void draw_solid_polygon(Span<const Vec2F> polygon, float radius, Color color);
    virtual void draw_circle(CircF circle, Color color);
    virtual void draw_solid_circle(CircF circle, float angle, Color color);
    virtual void draw_solid_capsule(const CapsuleF& capsule, Color color);
    virtual void draw_line(const SegmentF& segment, Color color);
    virtual void draw_transform(const PhysicsTransform& transform);
    virtual void draw_point(CircF point, Color color);
    virtual void draw_string(Vec2F point, std::string_view string, Color color);
  };

}

#endif // GF_PHYSICS_DEBUG_H
