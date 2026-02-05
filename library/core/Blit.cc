// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Blit.h>

namespace gf {

  Blit compute_blit(RectI origin_region, Vec2I origin_size, Vec2I target_offset, Vec2I target_size)
  {
    // clip origin

    if (origin_region.offset.x < 0) {
      origin_region.extent.w += origin_region.offset.x;
      target_offset.x -= origin_region.offset.x;
      origin_region.offset.x = 0;
    }

    if (origin_region.offset.y < 0) {
      origin_region.extent.h += origin_region.offset.y;
      target_offset.y -= origin_region.offset.y;
      origin_region.offset.y = 0;
    }

    const Vec2I max_size = origin_size - origin_region.offset;

    origin_region.extent.w = std::min(origin_region.extent.w, max_size.w);
    origin_region.extent.h = std::min(origin_region.extent.h, max_size.h);

    // clip target

    if (target_offset.x < 0) {
      origin_region.extent.w += target_offset.x;
      origin_region.offset.x -= target_offset.x;
      target_offset.x = 0;
    }

    if (target_offset.y < 0) {
      origin_region.extent.h += target_offset.y;
      origin_region.offset.y -= target_offset.y;
      target_offset.y = 0;
    }

    const Vec2I overwrite = target_offset + origin_region.extent - target_size;

    if (overwrite.w > 0) {
      origin_region.extent.w -= overwrite.w;
    }

    if (overwrite.h > 0) {
      origin_region.extent.h -= overwrite.h;
    }

    return { origin_region, target_offset };
  }

}
