// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_FILTER_H
#define GF_PHYSICS_FILTER_H

#include <cstdint>

#include <box2d/box2d.h>

#include "PhysicsApi.h"

namespace gf {

  struct GF_PHYSICS_API PhysicsFilter {
    uint64_t category_bits = B2_DEFAULT_CATEGORY_BITS;
    uint64_t mask_bits = B2_DEFAULT_MASK_BITS;
    int32_t group_index = 0;
  };

  struct GF_PHYSICS_API PhysicsQueryFilter {
    uint64_t category_bits = B2_DEFAULT_CATEGORY_BITS;
    uint64_t mask_bits = B2_DEFAULT_MASK_BITS;
  };

  namespace details {

    constexpr b2Filter to_raw(const PhysicsFilter& filter)
    {
      return { filter.category_bits, filter.mask_bits, filter.group_index };
    }

    constexpr PhysicsFilter to_filter(const b2Filter& raw)
    {
      return { raw.categoryBits, raw.maskBits, raw.groupIndex };
    }

    constexpr b2QueryFilter to_raw(const PhysicsQueryFilter& filter)
    {
      return { filter.category_bits, filter.mask_bits };
    }

    constexpr PhysicsQueryFilter to_filter(const b2QueryFilter& raw)
    {
      return { raw.categoryBits, raw.maskBits };
    }

  }

}

#endif // GF_PHYSICS_FILTER_H
