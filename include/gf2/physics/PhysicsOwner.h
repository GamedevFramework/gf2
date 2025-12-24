// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_OWNER_H
#define GF_PHYSICS_OWNER_H

#include <cstdint>

namespace gf::details {

  enum class PhysicsOwner : uint8_t {
    None,
    Object,
  };

}

#endif // GF_PHYSICS_OWNER_H
