// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/AnimationData.h>

namespace gf {

  void AnimationData::add_tileset(uint32_t texture_index, Vec2I layout, Time duration, int32_t frame_count, int32_t frame_offset)
  {
    int32_t x = frame_offset % layout.w;
    int32_t y = frame_offset / layout.w;
    const Vec2F size = 1.0f / layout;

    for (int index = 0; index < frame_count; ++index) {
      assert(y < layout.h);
      AnimationFrameData frame = {};
      frame.texture_index = texture_index;
      frame.texture_region = RectF::from_position_size(gf::vec(x, y) * size, size);
      frame.duration = duration;

      frames.push_back(frame);

      ++x;

      if (x == layout.w) {
        x = 0;
        ++y;
      }
    }
  }

}
