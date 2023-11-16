// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/physics/PhysicsWorld.h>
// clang-format on

#include <chipmunk/chipmunk.h>

namespace gf {

  PhysicsWorld::PhysicsWorld()
  : m_space(cpSpaceNew())
  {
  }

  PhysicsWorld::PhysicsWorld(details::PhysicsExistingType existing, cpSpace* space)
  : m_space(existing, space)
  {
  }

}
