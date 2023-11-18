// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_COLLISION_HANDLER_H
#define GF_PHYSICS_COLLISION_HANDLER_H

#include "PhysicsApi.h"

namespace gf {
  class PhysicsArbiter;
  class PhysicsWorld;

  class GF_PHYSICS_API PhysicsCollisionHandler {
  public:
    PhysicsCollisionHandler(const PhysicsCollisionHandler&) = default;
    PhysicsCollisionHandler(PhysicsCollisionHandler&&) noexcept = default;
    virtual ~PhysicsCollisionHandler() = default;

    PhysicsCollisionHandler& operator=(const PhysicsCollisionHandler&) = default;
    PhysicsCollisionHandler& operator=(PhysicsCollisionHandler&&) noexcept = default;

    virtual bool begin(PhysicsArbiter arbiter, PhysicsWorld world);
    virtual bool pre_solve(PhysicsArbiter arbiter, PhysicsWorld world);
    virtual void post_solve(PhysicsArbiter arbiter, PhysicsWorld world);
    virtual void separate(PhysicsArbiter arbiter, PhysicsWorld world);
  };

}

#endif // GF_PHYSICS_COLLISION_HANDLER_H
