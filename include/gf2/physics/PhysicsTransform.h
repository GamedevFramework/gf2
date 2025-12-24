// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_TRANSFORM_H
#define GF_PHYSICS_TRANSFORM_H

#include <cmath>

#include <box2d/box2d.h>

#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"
#include "box2d/math_functions.h"

namespace gf {

  struct GF_PHYSICS_API PhysicsRotation {
    constexpr PhysicsRotation() = default;

    PhysicsRotation(float angle)
    : cosine(std::cos(angle))
    , sine(std::sin(angle))
    {
    }

    constexpr PhysicsRotation(float c, float s)
    : cosine(c)
    , sine(s)
    {
    }

    float angle() const
    {
      return std::atan2(sine, cosine);
    }

    float cosine = 1.0f;
    float sine = 0.0f;
  };

  struct GF_PHYSICS_API PhysicsTransform {
    Vec2F location = { 0.0f, 0.0f };
    PhysicsRotation rotation;
  };

  namespace details {

    constexpr b2Transform to_raw(const PhysicsTransform& transform)
    {
      return { { transform.location.x, transform.location.y }, { transform.rotation.cosine, transform.rotation.sine } };
    }

    constexpr PhysicsTransform to_transform(const b2Transform& raw)
    {
      return { { raw.p.x, raw.p.y }, { raw.q.c, raw.q.s } };
    }

  }

}


#endif // GF_PHYSICS_TRANSFORM_H
