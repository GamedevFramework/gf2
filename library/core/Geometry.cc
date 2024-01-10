// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/core/Geometry.h>
#include "gf2/core/Polyline.h"
// clang-format on

#include <algorithm>
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

      const Vec2I first_point = iterator->front();
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

  Bresenham::Bresenham(Vec2I p0, Vec2I p1)
  : m_p0(p0)
  , m_p1(p1)
  , m_delta(p1 - p0)
  {
    m_step.x = gf::sign(m_delta.x);
    m_step.y = gf::sign(m_delta.y);
    m_error = std::max(m_step.x * m_delta.x, m_step.y * m_delta.y);
    m_delta *= 2;
  }

  std::optional<Vec2I> Bresenham::step()
  {
    if (m_step.x * m_delta.x > m_step.y * m_delta.y) {
      if (m_p0.x == m_p1.x) {
        return std::nullopt;
      }

      m_p0.x += m_step.x;
      m_error -= m_step.y * m_delta.y;

      if (m_error < 0) {
        m_p0.y += m_step.y;
        m_error += m_step.x * m_delta.x;
      }
    } else {
      if (m_p0.y == m_p1.y) {
        return std::nullopt;
      }

      m_p0.y += m_step.y;
      m_error -= m_step.x * m_delta.x;

      if (m_error < 0) {
        m_p0.x += m_step.x;
        m_error += m_step.y * m_delta.y;
      }
    }

    return m_p0;
  }

  std::vector<Vec2I> generate_line(Vec2I p0, Vec2I p1)
  {
    Bresenham bresenham(p0, p1);

    std::vector<Vec2I> line;
    line.push_back(p0);

    for (;;) {
      if (auto maybe_next = bresenham.step(); maybe_next) {
        line.push_back(*maybe_next);
      } else {
        break;
      }
    }

    assert(line.back() == p1);
    line.pop_back(); // to remove p1

    return line;
  }

}
