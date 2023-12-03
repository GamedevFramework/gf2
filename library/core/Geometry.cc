// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/core/Geometry.h>
#include "gf2/core/Polyline.h"
// clang-format on

#include <set>
#include <tuple>

namespace gf {

  namespace {

    template<typename Iterator>
    std::tuple<Iterator, bool> find_next_segment(Iterator begin, Iterator end, Vec2I end_point)
    {
      for (auto iterator = begin; iterator != end; ++iterator) {
        if (iterator->front() == end_point) {
          return std::make_tuple(iterator, false);
        }

        if (iterator->back() == end_point) {
          return std::make_tuple(iterator, true);
        }
      }

      return std::make_tuple(end, false);
    }

  }

  std::vector<Polyline> compute_lines(Span<const SegmentI> segments)
  {
    std::vector<Polyline> lines;

    auto segment_comparator = [](const SegmentI& lhs, const SegmentI& rhs) {
      return std::tie(lhs[0].x, lhs[0].y, lhs[1].x, lhs[1].y) < std::tie(rhs[0].x, rhs[0].y, rhs[1].x, rhs[1].y);
    };

    std::multiset<SegmentI, decltype(segment_comparator)> remaining(segments.begin(), segments.end(), segment_comparator);

    for (auto iterator = remaining.begin(); iterator != remaining.end(); /*  not done here */) {
      // start a new polyline
      Polyline polyline;

      Vec2I first_point = iterator->front();
      polyline.points.emplace_back(first_point);

      Vec2I current_point = iterator->back();

      for (;;) {
        polyline.points.emplace_back(current_point);

        auto [next_iterator, reverse] = find_next_segment(std::next(iterator), remaining.end(), current_point);

        if (next_iterator == remaining.end()) {
          polyline.type = PolylineType::Chain;
          break;
        }

        SegmentI next_segment = *next_iterator;
        remaining.erase(next_iterator);

        if (reverse) {
          std::swap(next_segment[0], next_segment[1]);
        }

        assert(next_segment[0] == current_point);
        current_point = next_segment[1];

        if (current_point == first_point) {
          polyline.type = PolylineType::Loop;
          break;
        }
      }

      iterator = remaining.erase(iterator);
      lines.emplace_back(std::move(polyline));
    }

    return lines;
  }

}
