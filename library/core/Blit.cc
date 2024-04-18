// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/Blit.h>

namespace gf {

  Blit compute_blit(RectI source_region, Vec2I source_size, Vec2I target_offset, Vec2I target_size)
  {
    // clip source

    if (source_region.offset.x < 0) {
      source_region.extent.w += source_region.offset.x;
      target_offset.x -= source_region.offset.x;
      source_region.offset.x = 0;
    }

    if (source_region.offset.y < 0) {
      source_region.extent.h += source_region.offset.y;
      target_offset.y -= source_region.offset.y;
      source_region.offset.y = 0;
    }

    const Vec2I max_size = source_size - source_region.offset;

    source_region.extent.w = std::min(source_region.extent.w, max_size.w);
    source_region.extent.h = std::min(source_region.extent.h, max_size.h);

    // clip target

    if (target_offset.x < 0) {
      source_region.extent.w += target_offset.x;
      source_region.offset.x -= target_offset.x;
      target_offset.x = 0;
    }

    if (target_offset.y < 0) {
      source_region.extent.h += target_offset.y;
      source_region.offset.y -= target_offset.y;
      target_offset.y = 0;
    }

    const Vec2I overwrite = target_offset + source_region.extent - target_size;

    if (overwrite.w > 0) {
      source_region.extent.w -= overwrite.w;
    }

    if (overwrite.h > 0) {
      source_region.extent.h -= overwrite.h;
    }

    return { source_region, target_offset };
  }

}
