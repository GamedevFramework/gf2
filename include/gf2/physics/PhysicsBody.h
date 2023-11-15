// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_BODY_H
#define GF_PHYSICS_BODY_H

#include "PhysicsHandle.h"

// NOLINTBEGIN
struct cpBody;
extern "C" {
  void* cpBodyGetUserData(const cpBody *space);
  void cpBodySetUserData(cpBody *space, void* value);
  void cpBodyDestroy(cpBody *space);
}
// NOLINTEND

namespace gf {

  class PhysicsBody {
  public:
    PhysicsBody();

  private:
    details::PhysicsHandle<cpBody, cpBodyGetUserData, cpBodySetUserData, cpBodyDestroy> m_body;
  };

}

#endif // GF_PHYSICS_BODY_H
