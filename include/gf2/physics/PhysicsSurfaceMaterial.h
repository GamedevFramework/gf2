// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_SURFACE_MATERIAL_H
#define GF_PHYSICS_SURFACE_MATERIAL_H

#include <cstdint>

#include <box2d/box2d.h>

#include <gf2/core/Color.h>

#include "PhysicsApi.h"

namespace gf {

  struct GF_PHYSICS_API PhysicsSurfaceMaterial {
    float friction = 0.6f;
    float restitution = 0.0f;
    float rolling_resistance = 0.0f;
    float tangent_speed = 0.0f;
    uint64_t user_material_id = 0;
    Color custom_color = gf::Black;
  };

  namespace details {

    inline b2SurfaceMaterial to_raw(const PhysicsSurfaceMaterial& material)
    {
      b2SurfaceMaterial raw = b2DefaultSurfaceMaterial();
      raw.friction = material.friction;
      raw.restitution = material.restitution;
      raw.rollingResistance = material.rolling_resistance;
      raw.tangentSpeed = material.tangent_speed;
      raw.userMaterialId = material.user_material_id;
      raw.customColor = material.custom_color.to_hex() >> 8;
      return raw;
    }

    inline PhysicsSurfaceMaterial to_material(const b2SurfaceMaterial& raw)
    {
      return { raw.friction, raw.restitution, raw.rollingResistance, raw.tangentSpeed, raw.userMaterialId, raw.customColor };
    }

  }

}

#endif // GF_PHYSICS_SURFACE_MATERIAL_H
