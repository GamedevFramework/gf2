// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_WORLD_H
#define GF_PHYSICS_WORLD_H

#include "PhysicsHandle.h"

// NOLINTBEGIN
struct cpSpace;
extern "C" {
  void* cpSpaceGetUserData(const cpSpace *space);
  void cpSpaceSetUserData(cpSpace *space, void* value);
  void cpSpaceDestroy(cpSpace *space);
}
// NOLINTEND

namespace gf {

  class PhysicsWorld {
  public:
    PhysicsWorld();

  private:
    details::PhysicsHandle<cpSpace, cpSpaceGetUserData, cpSpaceSetUserData, cpSpaceDestroy> m_space;
  };

}

#endif // GF_PHYSICS_WORLD_H
