// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_SHAPE_H
#define GF_PHYSICS_SHAPE_H

#include "PhysicsHandle.h"

// NOLINTBEGIN
struct cpShape;
extern "C" {
  void* cpShapeGetUserData(const cpShape *space);
  void cpShapeSetUserData(cpShape *space, void* value);
  void cpShapeDestroy(cpShape *space);
}
// NOLINTEND

namespace gf {

  class PhysicsShape {
  public:
    PhysicsShape();

  private:
    details::PhysicsHandle<cpShape, cpShapeGetUserData, cpShapeSetUserData, cpShapeDestroy> m_shape;
  };

}

#endif // GF_PHYSICS_SHAPE_H
