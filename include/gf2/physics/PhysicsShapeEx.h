// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_SHAPE_EX_H
#define GF_PHYSICS_SHAPE_EX_H

#include <vector>

#include <gf2/core/Polyline.h>
#include <gf2/core/Span.h>
#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"
#include "PhysicsShape.h"

namespace gf {
  class PhysicsBody;

  GF_PHYSICS_API std::vector<PhysicsShape> make_polyline_shapes(PhysicsBody* body, Span<const Vec2F> polyline, float radius, PolylineType type = PolylineType::Chain);
  GF_PHYSICS_API std::vector<PhysicsShape> make_polyline_shapes(PhysicsBody* body, const Polyline& polyline, float radius);

}

#endif // GF_PHYSICS_SHAPE_EX_H
