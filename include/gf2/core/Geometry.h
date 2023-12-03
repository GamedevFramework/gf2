// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_GEOMETRY_H
#define GF_GEOMETRY_H

#include <array>
#include <vector>

#include "Polyline.h"
#include "Span.h"
#include "Vec2.h"

namespace gf {

  template<typename T>
  using Segment = std::array<Vec2<T>, 2>;

  using SegmentI = Segment<int32_t>;
  using SegmentF = Segment<float>;

  std::vector<Polyline> compute_lines(Span<const SegmentI> segments);

}

#endif // GF_GEOMETRY_H
