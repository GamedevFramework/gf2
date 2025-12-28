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
    PhysicsManifold manifold = {};
  };

  namespace details {

    inline PhysicsContactFeatures to_contact_features(const b2ContactData& raw)
    {
      PhysicsContactFeatures features;
      features.shape0 = PhysicsShape::from_id(details::to_id(raw.shapeIdA));
      features.shape1 = PhysicsShape::from_id(details::to_id(raw.shapeIdB));
      features.contact = PhysicsContact::from_id(details::to_id(raw.contactId));

      features.manifold.normal = { raw.manifold.normal.x, raw.manifold.normal.y };
      features.manifold.rolling_impulse = raw.manifold.rollingImpulse;
      features.manifold.point_count = raw.manifold.pointCount;
      assert(features.manifold.point_count < 2);

      for (int i = 0; i < raw.manifold.pointCount; ++i) {
        const b2ManifoldPoint& raw_point = raw.manifold.points[i];
        PhysicsManifoldPoint& point = features.manifold.points[i];

        point.point = { raw_point.point.x, raw_point.point.y };
        point.anchor0 = { raw_point.anchorA.x, raw_point.anchorA.y };
        point.anchor1 = { raw_point.anchorB.x, raw_point.anchorB.y };
        point.separation = raw_point.separation;
        point.normal_impulse = raw_point.normalImpulse;
        point.tangent_impulse = raw_point.tangentImpulse;
        point.total_normal_impulse = raw_point.totalNormalImpulse;
        point.normal_velocity = raw_point.normalVelocity;
        point.id = raw_point.id;
        point.persisted = raw_point.persisted;
      }

      return features;
    }

  }

}

#endif // GF_PHYSICS_CONTACT_H
