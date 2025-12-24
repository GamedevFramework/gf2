// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_CONTACT_H
#define GF_PHYSICS_CONTACT_H

#include <cstdint>

#include <array>

#include <box2d/box2d.h>

#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"
#include "PhysicsId.h"
#include "PhysicsShape.h"

namespace gf {
  struct PhysicsContactFeatures;

  class GF_PHYSICS_API PhysicsContact {
  public:
    PhysicsContact() = default;

    PhysicsContactId id() const;
    static PhysicsContact from_id(PhysicsContactId id);

    bool valid() const;

    PhysicsContactFeatures features() const;

  private:
    constexpr PhysicsContact(b2ContactId id)
    : m_id(id)
    {
    }

    b2ContactId m_id = {};
  };

  struct GF_PHYSICS_API PhysicsManifoldPoint {
    Vec2F point;
    Vec2F anchor0;
    Vec2F anchor1;
    float separation;
    float normal_impulse;
    float tangent_impulse;
    float total_normal_impulse;
    float normal_velocity;
    uint16_t id;
    bool persisted;
  };

  struct GF_PHYSICS_API PhysicsManifold {
    Vec2F normal;
    float rolling_impulse;
    std::array<PhysicsManifoldPoint, 2> points;
    std::size_t point_count;
  };

  struct GF_PHYSICS_API PhysicsContactFeatures {
    PhysicsContact contact;
    PhysicsShape shape0;
    PhysicsShape shape1;
    PhysicsManifold manifold;
  };

}

#endif // GF_PHYSICS_CONTACT_H
