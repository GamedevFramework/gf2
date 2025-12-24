// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_MASS_DATA_H
#define GF_PHYSICS_MASS_DATA_H

#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"

namespace gf {

  struct GF_PHYSICS_API PhysicsMassData {
    float mass = 0.0f;
    Vec2F center = { 0.0f, 0.0f };
    float rotational_inertia = 0.0f;
  };

}

#endif // GF_PHYSICS_MASS_DATA_H
