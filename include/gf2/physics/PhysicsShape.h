// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_SHAPE_H
#define GF_PHYSICS_SHAPE_H

#include <chipmunk/chipmunk.h>

#include "PhysicsApi.h"
#include "PhysicsHandle.h"

namespace gf {

  class GF_PHYSICS_API PhysicsShape {
  public:
    PhysicsShape() = default;

  protected:
    PhysicsShape(cpShape* shape);

  private:
    friend class PhysicsBody;

    PhysicsShape(details::PhysicsExistingType existing, cpShape* shape);

    details::PhysicsHandle<cpShape, cpShapeGetUserData, cpShapeSetUserData, cpShapeDestroy> m_shape;
  };

}

#endif // GF_PHYSICS_SHAPE_H
