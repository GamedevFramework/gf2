// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/BinPack.h>

#include <cassert>

#include <algorithm>
#include <limits>
#include <numeric>

namespace gf {
  // inspired by https://github.com/juj/RectangleBinPack (public domain)
  // article: A Thousand Ways to Pack the Bin - A Practical Approach to Two-Dimensional Rectangle Bin Packing, Jukka Jylänki, Feb 2010
  // http://pds25.egloos.com/pds/201504/21/98/RectangleBinPack.pdf

  namespace {
    int score_best_area(Vec2I size, RectI rectangle)
    {
      return rectangle.extent.w * rectangle.extent.h - size.w * size.h;
    }

    int score_best_short_side(Vec2I size, RectI rectangle)
    {
      return std::min(std::abs(rectangle.extent.w - size.w), std::abs(rectangle.extent.h - size.h));
    }

    int score_best_long_side(Vec2I size, RectI rectangle)
    {
      return std::max(std::abs(rectangle.extent.w - size.w), std::abs(rectangle.extent.h - size.h));
    }

    int choice_heuristic_score(Vec2I size, RectI rectangle, BinPackChoice choice)
    {
      switch (choice) {
        case BinPackChoice::BestArea:
          return score_best_area(size, rectangle);
        case BinPackChoice::BestShortSide:
          return score_best_short_side(size, rectangle);
        case BinPackChoice::BestLongSide:
          return score_best_long_side(size, rectangle);
        case BinPackChoice::WorstArea:
          return -score_best_area(size, rectangle);
        case BinPackChoice::WorstShortSide:
          return -score_best_short_side(size, rectangle);
        case BinPackChoice::WorstLongSide:
          return -score_best_long_side(size, rectangle);
      }

      assert(false);
      return std::numeric_limits<int>::max();
    }

    bool split_heuristic_horizontal(RectI free_rectangle, RectI new_rectangle, BinPackSplit split)
    {
      const Vec2I leftover = free_rectangle.extent - new_rectangle.extent;

      switch (split) {
        case BinPackSplit::ShorterLeftoverAxis:
          return leftover.w < leftover.h;
        case BinPackSplit::LongerLeftoverAxis:
          return leftover.w > leftover.h;
        case BinPackSplit::MinimumArea:
          return leftover.w * new_rectangle.extent.h < new_rectangle.extent.w * leftover.h;
        case BinPackSplit::MaximumArea:
          return leftover.w * new_rectangle.extent.h > new_rectangle.extent.w * leftover.h;
        case BinPackSplit::ShorterAxis:
          return new_rectangle.extent.w < new_rectangle.extent.h;
        case BinPackSplit::LongerAxis:
          return new_rectangle.extent.w > new_rectangle.extent.h;
      }

      assert(false);
      return true;
    }

    using SizeComparator = bool (*)(Vec2I, Vec2I);

    SizeComparator sort_heuristic_comparator(BinPackSort sort)
    {
      switch (sort) {
        case BinPackSort::AreaAsc:
          return [](Vec2I lhs, Vec2I rhs) {
            return lhs.w * lhs.h < rhs.w * rhs.h;
          };
        case BinPackSort::AreaDesc:
          return [](Vec2I lhs, Vec2I rhs) {
            return lhs.w * lhs.h > rhs.w * rhs.h;
          };
        case BinPackSort::ShortSideAsc:
          return [](Vec2I lhs, Vec2I rhs) {
            auto [lhs_min_side, lhs_max_side] = std::minmax(lhs.w, lhs.h);
            auto [rhs_min_side, rhs_max_side] = std::minmax(rhs.w, rhs.h);
            return lhs_min_side < rhs_min_side || (lhs_min_side == rhs_min_side && lhs_max_side < rhs_max_side);
          };
        case BinPackSort::ShortSideDesc:
          return [](Vec2I lhs, Vec2I rhs) {
            auto [lhs_min_side, lhs_max_side] = std::minmax(lhs.w, lhs.h);
            auto [rhs_min_side, rhs_max_side] = std::minmax(rhs.w, rhs.h);
            return lhs_min_side > rhs_min_side || (lhs_min_side == rhs_min_side && lhs_max_side > rhs_max_side);
          };
        case BinPackSort::LongSideAsc:
          return [](Vec2I lhs, Vec2I rhs) {
            auto [lhs_min_side, lhs_max_side] = std::minmax(lhs.w, lhs.h);
            auto [rhs_min_side, rhs_max_side] = std::minmax(rhs.w, rhs.h);
            return lhs_max_side < rhs_max_side || (lhs_max_side == rhs_max_side && lhs_min_side < rhs_min_side);
          };
        case BinPackSort::LongSideDesc:
          return [](Vec2I lhs, Vec2I rhs) {
            auto [lhs_min_side, lhs_max_side] = std::minmax(lhs.w, lhs.h);
            auto [rhs_min_side, rhs_max_side] = std::minmax(rhs.w, rhs.h);
            return lhs_max_side > rhs_max_side || (lhs_max_side == rhs_max_side && lhs_min_side > rhs_min_side);
          };
        case BinPackSort::PerimeterAsc:
          return [](Vec2I lhs, Vec2I rhs) {
            return lhs.w + lhs.h < rhs.w + rhs.h;
          };
        case BinPackSort::PermiterDesc:
          return [](Vec2I lhs, Vec2I rhs) {
            return lhs.w + lhs.h > rhs.w + rhs.h;
          };
        case BinPackSort::SideDifferenceAsc:
          return [](Vec2I lhs, Vec2I rhs) {
            return std::abs(lhs.w - lhs.h) < std::abs(rhs.w - rhs.h);
          };
        case BinPackSort::SideDifferenceDesc:
          return [](Vec2I lhs, Vec2I rhs) {
            return std::abs(lhs.w - lhs.h) > std::abs(rhs.w - rhs.h);
          };
        case BinPackSort::SideRatioAsc:
          return [](Vec2I lhs, Vec2I rhs) {
            return lhs.w * rhs.h < rhs.w * lhs.h; // <=> lhs.w / lhs.h < rhs.w / rhs.h
          };
        case BinPackSort::SideRatioDesc:
          return [](Vec2I lhs, Vec2I rhs) {
            return lhs.w * rhs.h > rhs.w * lhs.h; // <=> lhs.w / lhs.h > rhs.w / rhs.h
          };
      }

      assert(false);
      return nullptr;
    }

  }

  BinPack::BinPack(Vec2I size)
  {
    m_free_rectangles.push_back(RectI::from_size(size));
  }

  std::optional<RectI> BinPack::insert(Vec2I size, BinPackChoice choice, BinPackSplit split)
  {
    auto [index, rectangle] = find_rectangle(size, choice);

    if (index == std::numeric_limits<std::size_t>::max()) {
      return std::nullopt;
    }

    split_rectangle(index, rectangle, split);
    rectangle_merge();
    return rectangle;
  }

  std::optional<std::vector<RectI>> BinPack::insert(const std::vector<Vec2I>& sizes, BinPackChoice choice, BinPackSplit split, BinPackSort sort)
  {
    auto saved_free_rectangles = m_free_rectangles;

    std::vector<std::size_t> indices(sizes.size());
    std::iota(indices.begin(), indices.end(), 0);

    // sort

    auto comparator = sort_heuristic_comparator(sort);

    std::sort(indices.begin(), indices.end(), [&sizes, &comparator](std::size_t lhs, std::size_t rhs) {
      return comparator(sizes[lhs], sizes[rhs]);
    });

    // insert all rectangles

    std::vector<RectI> rectangles(sizes.size());

    for (auto index : indices) {
      auto maybe_rectangle = insert(sizes[index], choice, split);

      if (!maybe_rectangle) {
        m_free_rectangles = saved_free_rectangles;
        return std::nullopt;
      }

      rectangles[index] = *maybe_rectangle;
    }

    return rectangles;
  }

  std::tuple<std::size_t, RectI> BinPack::find_rectangle(Vec2I size, BinPackChoice choice) const
  {
    int best_score = std::numeric_limits<int>::max();
    RectI best_rectangle = {};
    std::size_t best_index = std::numeric_limits<std::size_t>::max();

    for (std::size_t i = 0; i < m_free_rectangles.size(); ++i) {
      auto current_rectangle = m_free_rectangles[i];

      if (current_rectangle.size() == size) { // perfect match
        return std::make_tuple(i, current_rectangle);
      }

      if (size.w <= current_rectangle.extent.w && size.h <= current_rectangle.extent.h) { // fits
        const int score = choice_heuristic_score(size, current_rectangle, choice);

        if (score < best_score) {
          best_score = score;
          best_rectangle = RectI::from_position_size(current_rectangle.position(), size);
          best_index = i;
        }
      }
    }

    return std::make_tuple(best_index, best_rectangle);
  }

  void BinPack::split_rectangle(std::size_t index, RectI rectangle, BinPackSplit split)
  {
    const RectI free_rectangle = m_free_rectangles[index];

    RectI bottom;
    bottom.offset.x = free_rectangle.offset.x;
    bottom.offset.y = free_rectangle.offset.y + rectangle.extent.h;
    bottom.extent.h = free_rectangle.extent.h - rectangle.extent.h;

    RectI right;
    right.offset.x = free_rectangle.offset.x + rectangle.extent.w;
    right.offset.y = free_rectangle.offset.y;
    right.extent.w = free_rectangle.extent.w - rectangle.extent.w;

    if (split_heuristic_horizontal(free_rectangle, rectangle, split)) {
      bottom.extent.w = free_rectangle.extent.w;
      right.extent.h = rectangle.extent.h;
    } else {
      bottom.extent.w = rectangle.extent.w;
      right.extent.h = free_rectangle.extent.h;
    }

    if (!bottom.empty()) {
      m_free_rectangles.push_back(bottom);
    }

    if (!right.empty()) {
      m_free_rectangles.push_back(right);
    }

    remove_rectangle(index);
  }

  void BinPack::rectangle_merge()
  {
    for (std::size_t i = 0; i < m_free_rectangles.size(); ++i) {
      RectI ri = m_free_rectangles[i];

      for (std::size_t j = i + 1; j < m_free_rectangles.size(); ++j) {
        const RectI rj = m_free_rectangles[j];

        if (ri.extent.w == rj.extent.w && ri.offset.x == rj.offset.x) {
          if (ri.offset.y == rj.offset.y + rj.extent.h) {
            ri.offset.y -= rj.extent.h;
            ri.extent.h += rj.extent.h;
            remove_rectangle(j);
            --j;
            m_free_rectangles[i] = ri;
          } else if (ri.offset.y + ri.extent.h == rj.offset.y) {
            ri.extent.h += rj.extent.h;
            remove_rectangle(j);
            --j;
            m_free_rectangles[i] = ri;
          }
        } else if (ri.extent.h == rj.extent.h && ri.offset.y == rj.offset.y) {
          if (ri.offset.x == rj.offset.x + rj.extent.w) {
            ri.offset.x -= rj.extent.w;
            ri.extent.w += rj.extent.w;
            remove_rectangle(j);
            --j;
            m_free_rectangles[i] = ri;
          } else if (ri.offset.x + ri.extent.w == rj.offset.x) {
            ri.extent.w += rj.extent.w;
            remove_rectangle(j);
            --j;
            m_free_rectangles[i] = ri;
          }
        }
      }
    }
  }

  void BinPack::remove_rectangle(std::size_t index)
  {
    std::swap(m_free_rectangles[index], m_free_rectangles.back());
    m_free_rectangles.pop_back();
  }

}
