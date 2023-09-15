// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_BLIT_H
#define GF_BLIT_H

#include "CoreApi.h"
#include "Rect.h"
#include "Vec2.h"

namespace gf {

  struct GF_CORE_API Blit {
    RectI source_region;
    Vec2I target_offset;
  };

  GF_CORE_API Blit compute_blit(RectI source_region, Vec2I source_size, Vec2I target_offset, Vec2I target_size);

}

#endif // GF_BLIT_H
