// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_WORLD_H
#define GF_PHYSICS_WORLD_H

#include <chipmunk/chipmunk.h>

#include "PhysicsApi.h"
#include "PhysicsHandle.h"

namespace gf {

  class GF_PHYSICS_API PhysicsWorld {
  public:
    PhysicsWorld();

  private:
    friend class PhysicsBody;

    PhysicsWorld(details::PhysicsExistingType existing, cpSpace* space);

    details::PhysicsHandle<cpSpace, cpSpaceGetUserData, cpSpaceSetUserData, cpSpaceDestroy> m_space;
  };

}

#endif // GF_PHYSICS_WORLD_H
