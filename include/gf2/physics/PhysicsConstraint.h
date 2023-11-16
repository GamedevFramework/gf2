// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_CONSTRAINT_H
#define GF_PHYSICS_CONSTRAINT_H

#include <chipmunk/chipmunk.h>

#include "PhysicsHandle.h"

namespace gf {

  class PhysicsConstraint {
  public:
    PhysicsConstraint() = default;

  protected:
    PhysicsConstraint(cpConstraint* constraint);

  private:
    friend class PhysicsBody;

    PhysicsConstraint(details::PhysicsExistingType existing, cpConstraint* constraint);

    details::PhysicsHandle<cpConstraint, cpConstraintGetUserData, cpConstraintSetUserData, cpConstraintDestroy> m_constraint;
  };

}

#endif // GF_PHYSICS_CONSTRAINT_H
