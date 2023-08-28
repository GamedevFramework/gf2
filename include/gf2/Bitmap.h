// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_BITMAP_H
#define GF_BITMAP_H

#include <cstddef>
#include <cstdint>

#include <vector>

#include "CoreApi.h"
#include "Range.h"
#include "Vec2.h"

namespace gf {

  class GF_CORE_API Bitmap {
  public:
    Bitmap();
    Bitmap(Vec2I size);
    Bitmap(Vec2I size, uint8_t gray);
    Bitmap(Vec2I size, const uint8_t* source, ptrdiff_t pitch);

    Vec2I size() const;
    PositionRange position_range() const;

    uint8_t operator()(Vec2I position) const;

    std::size_t raw_size() const;
    const uint8_t* raw_data() const;

  private:
    std::ptrdiff_t offset_from_position(Vec2I position) const;

    Vec2I m_size;
    std::vector<uint8_t> m_pixels;
  };

}

#endif // BITMAP
