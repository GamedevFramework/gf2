// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/physics/PhysicsSpace.h>
// clang-format on

#include <chipmunk/chipmunk.h>

namespace gf {

  PhysicsSpace::PhysicsSpace()
  : m_space(cpSpaceNew())
  {
  }



}
