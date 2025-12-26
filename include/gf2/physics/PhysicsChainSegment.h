// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_CHAIN_SEGMENT_H
#define GF_PHYSICS_CHAIN_SEGMENT_H

#include <gf2/core/Geometry.h>

#include "PhysicsApi.h"

namespace gf {

  struct GF_PHYSICS_API PhysicsChainSegment {
    Vec2F tail_ghost;
    SegmentF segment;
    Vec2F head_ghost;
  };

}

#endif // GF_PHYSICS_CHAIN_SEGMENT_H
