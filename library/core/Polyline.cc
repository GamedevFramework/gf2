// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Polyline.h>

namespace gf {

  bool PolylineView::has_prev(std::size_t i) const
  {
    return type == PolylineType::Loop || i > 0;
  }

  Vec2F PolylineView::prev_point(std::size_t i) const
  {
    assert(has_prev(i));

    if (i == 0) {
      return points.back();
    }

    return points[i - 1];
  }

  bool PolylineView::has_next(std::size_t i) const
  {
    return type == PolylineType::Loop || i < points.size() - 1;
  }

  Vec2F PolylineView::next_point(std::size_t i) const
  {
    assert(has_next(i));

    if (i == points.size() - 1) {
      return points.front();
    }

    return points[i + 1];
  }

}
