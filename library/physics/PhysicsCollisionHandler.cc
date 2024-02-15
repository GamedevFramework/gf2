// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/physics/PhysicsCollisionHandler.h>

#include <gf2/physics/PhysicsArbiter.h>
#include <gf2/physics/PhysicsWorld.h>

namespace gf {

  bool PhysicsCollisionHandler::begin([[maybe_unused]] PhysicsArbiter arbiter, [[maybe_unused]] PhysicsWorld world) // NOLINT(performance-unnecessary-value-param)
  {
    return true;
  }

  bool PhysicsCollisionHandler::pre_solve([[maybe_unused]] PhysicsArbiter arbiter, [[maybe_unused]] PhysicsWorld world) // NOLINT(performance-unnecessary-value-param)
  {
    return true;
  }

  void PhysicsCollisionHandler::post_solve([[maybe_unused]] PhysicsArbiter arbiter, [[maybe_unused]] PhysicsWorld world) // NOLINT(performance-unnecessary-value-param)
  {
  }

  void PhysicsCollisionHandler::separate([[maybe_unused]] PhysicsArbiter arbiter, [[maybe_unused]] PhysicsWorld world) // NOLINT(performance-unnecessary-value-param)
  {
  }

}
