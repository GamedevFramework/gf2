// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/physics/PhysicsConstraint.h>
// clang-format on

#include <chipmunk/chipmunk.h>

namespace gf {

  PhysicsConstraint::PhysicsConstraint(cpConstraint* constraint)
  : m_constraint(constraint)
  {
  }

  PhysicsConstraint::PhysicsConstraint(details::PhysicsExistingType existing, cpConstraint* constraint)
  : m_constraint(existing, constraint)
  {
  }

}
