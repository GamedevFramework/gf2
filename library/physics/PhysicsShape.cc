// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/physics/PhysicsShape.h>
// clang-format on

namespace gf {

  PhysicsShape::PhysicsShape(cpShape* shape)
  : m_shape(shape)
  {
  }

  PhysicsShape::PhysicsShape(details::PhysicsExistingType existing, cpShape* shape)
  : m_shape(existing, shape)
  {
  }

}
