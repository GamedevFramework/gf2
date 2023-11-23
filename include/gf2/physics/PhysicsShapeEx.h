// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_SHAPE_EX_H
#define GF_PHYSICS_SHAPE_EX_H

#include <cstdint>
#include <vector>

#include <gf2/core/Span.h>
#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"
#include "PhysicsShape.h"

namespace gf {
  class PhysicsBody;

  enum class PolylineType : uint8_t {
    Chain,
    Loop,
  };

  GF_PHYSICS_API std::vector<PhysicsShape> make_polyline_shapes(PhysicsBody* body, Span<const Vec2F> polyline, float radius, PolylineType type = PolylineType::Chain);

}

#endif // GF_PHYSICS_SHAPE_EX_H
