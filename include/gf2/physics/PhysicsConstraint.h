// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_CONSTRAINT_H
#define GF_PHYSICS_CONSTRAINT_H

#include "PhysicsHandle.h"

// NOLINTBEGIN
struct cpConstraint;
extern "C" {
void* cpConstraintGetUserData(const cpConstraint* space);
void cpConstraintSetUserData(cpConstraint* space, void* value);
void cpConstraintDestroy(cpConstraint* space);
}
// NOLINTEND

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
