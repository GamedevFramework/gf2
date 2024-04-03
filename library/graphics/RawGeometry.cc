// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/RawGeometry.h>

#include <numeric>

namespace gf {

  RectF RawGeometry::compute_bounds() const
  {
    if (vertices.empty()) {
      return RectF::from_size({ 0.0f, 0.0f });
    }

    const Vec2F first = vertices.front().location;

    const Vec2F min = std::accumulate(vertices.begin(), vertices.end(), first, [](Vec2F lhs, const Vertex& rhs) {
      return gf::min(lhs, rhs.location);
    });

    const Vec2F max = std::accumulate(vertices.begin(), vertices.end(), first, [](Vec2F lhs, const Vertex& rhs) {
      return gf::max(lhs, rhs.location);
    });

    return RectF::from_min_max(min, max);
  }

}
