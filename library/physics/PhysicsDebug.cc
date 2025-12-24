// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/physics/PhysicsDebug.h>

namespace gf {

  PhysicsDebug::~PhysicsDebug() = default;

  void PhysicsDebug::draw_polygon([[maybe_unused]] Span<const Vec2F> polygon, [[maybe_unused]] Color color) {
    // nothing to do by default
  }

  void PhysicsDebug::draw_solid_polygon([[maybe_unused]] Span<const Vec2F> polygon, [[maybe_unused]] float radius, [[maybe_unused]] Color color) {
    // nothing to do by default
  }

  void PhysicsDebug::draw_circle([[maybe_unused]] CircF circle, [[maybe_unused]] Color color) {
    // nothing to do by default
  }

  void PhysicsDebug::draw_solid_circle([[maybe_unused]] CircF circle, [[maybe_unused]] float angle, [[maybe_unused]] Color color) {
    // nothing to do by default
  }

  void PhysicsDebug::draw_solid_capsule([[maybe_unused]] const CapsuleF& capsule, [[maybe_unused]] Color color) {
    // nothing to do by default
  }

  void PhysicsDebug::draw_line([[maybe_unused]] const SegmentF& segment, [[maybe_unused]] Color color) {
    // nothing to do by default
  }

  void PhysicsDebug::draw_transform([[maybe_unused]] const PhysicsTransform& transform) {
    // nothing to do by default
  }

  void PhysicsDebug::draw_point([[maybe_unused]] CircF point, [[maybe_unused]] Color color) {
    // nothing to do by default
  }

  void PhysicsDebug::draw_string([[maybe_unused]] Vec2F point, [[maybe_unused]] std::string_view string, [[maybe_unused]] Color color) {
    // nothing to do by default
  }


}
