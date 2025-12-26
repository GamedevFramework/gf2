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
    return details::to_contact_features(b2Contact_GetData(m_id));
  }

}
