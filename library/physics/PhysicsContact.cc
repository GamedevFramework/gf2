// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/physics/PhysicsContact.h>

namespace gf {

  PhysicsContactId PhysicsContact::id() const
  {
    return details::to_id(m_id);
  }

  PhysicsContact PhysicsContact::from_id(PhysicsContactId id)
  {
    return { details::to_raw(id) };
  }

  bool PhysicsContact::valid() const
  {
    return b2Contact_IsValid(m_id);
  }

  PhysicsContactFeatures PhysicsContact::features() const
  {
    b2ContactData raw = b2Contact_GetData(m_id);

    PhysicsContactFeatures features;
    features.shape0 = PhysicsShape::from_id(details::to_id(raw.shapeIdA));
    features.shape1 = PhysicsShape::from_id(details::to_id(raw.shapeIdB));
    features.contact = PhysicsContact::from_id(details::to_id(raw.contactId));

    features.manifold.normal = { raw.manifold.normal.x, raw.manifold.normal.y };
    features.manifold.rolling_impulse = raw.manifold.rollingImpulse;
    features.manifold.point_count = raw.manifold.pointCount;

    return features;
  }

}
