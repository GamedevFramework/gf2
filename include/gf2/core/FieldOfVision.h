// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_FIELD_OF_VISION_H
#define GF_FIELD_OF_VISION_H

#include <cassert>
#include <queue>

#include "Array2D.h"
#include "Direction.h"
#include "Rational.h"
#include "Vec2.h"

namespace gf {

  /*
   * Symmetric Shadowcasting
   * based on https://github.com/370417/symmetric-shadowcasting
   * License: CC0-1.0
   */

  namespace details {

    template<typename T>
    T round_ties_up(const Rational<T>& rat)
    {
      T q = rat.numerator() / rat.denominator();
      T r = rat.numerator() % rat.denominator();

      while (r < 0) {
        r += rat.denominator();
        --q;
      }

      if (2 * r >= rat.denominator()) {
        return q + 1;
      }

      return q;
    }

    template<typename T>
    T round_ties_down(const Rational<T>& rat)
    {
      T q = rat.numerator() / rat.denominator();
      T r = rat.numerator() % rat.denominator();

      while (r < 0) {
        r += rat.denominator();
        --q;
      }

      if (2 * r > rat.denominator()) {
        return q + 1;
      }

      return q;
    }

    struct Quadrant {
      Direction direction;
      Vec2I origin;

      Vec2I transform(Vec2I position) const
      {
        switch (direction) {
          case Direction::Up:
            return { origin.x + position.x, origin.y - position.y };
          case Direction::Down:
            return { origin.x + position.x, origin.y + position.y };
          case Direction::Right:
            return { origin.x + position.y, origin.y + position.x };
          case Direction::Left:
            return { origin.x - position.y, origin.y + position.x };

          default:
            break;
        }

        assert(false);
        return { 0, 0 };
      }
    };

    struct Row {
      int32_t depth = 0;
      Rational<int32_t> start_slope;
      Rational<int32_t> end_slope;

      PositionRange tiles() const
      {
        auto min_x = round_ties_down(depth * start_slope);
        auto max_x = round_ties_up(depth * end_slope);
        return rectangle_range(RectI::from_position_size({ min_x, depth }, { max_x - min_x + 1, 1 }));
      }

      Row next() const
      {
        return { depth + 1, start_slope, end_slope };
      }
    };

    inline bool is_symmetric(const Row& row, Vec2I position)
    {
      return row.depth * row.start_slope <= position.x && position.x <= row.depth * row.end_slope;
    }

    inline Rational<int32_t> slope(Vec2I position)
    {
      return { (2 * position.x) - 1, 2 * position.y };
    }

    template<typename InputCell, typename OutputCell, typename Function>
    void compute_visibility_in_quadrant(const Array2D<InputCell>& input_cells, Array2D<OutputCell>& output_cells, Vec2I origin, int range_limit, Quadrant quadrant, Function set_visible) // NOLINT(readability-function-cognitive-complexity)
    {
      const int square_range_limit = square(range_limit);
      const Row first_row = { 1, -1, 1 };

      std::queue<Row> rows;
      rows.push(first_row);

      while (!rows.empty()) {
        Row row = rows.front();
        rows.pop();

        std::optional<Vec2I> prev_absolute;

        const PositionRange range = row.tiles();

        for (const Vec2I position : range) {
          const Vec2I absolute = quadrant.transform(position);

          if (square_distance(absolute, origin) > square_range_limit) {
            continue;
          }

          if (!input_cells.valid(absolute)) {
            continue;
          }

          const InputCell& input_cell = input_cells(absolute);
          OutputCell& output_cell = output_cells(absolute);

          if (!input_cell.transparent() || is_symmetric(row, position)) {
            set_visible(output_cell);
          }

          if (prev_absolute) {
            const InputCell& prev_input_cell = input_cells(*prev_absolute);

            if (!prev_input_cell.transparent() && input_cell.transparent()) {
              row.start_slope = slope(position);
            }

            if (prev_input_cell.transparent() && !input_cell.transparent()) {
              Row next_row = row.next();
              next_row.end_slope = slope(position);
              rows.push(next_row);
            }
          }

          prev_absolute = absolute;
        }

        if (prev_absolute && input_cells(*prev_absolute).transparent()) {
          rows.push(row.next());
        }
      }
    }

  }

  template<typename InputCell, typename OutputCell, typename Function>
  void compute_symmetric_shadowcasting(const Array2D<InputCell>& input_cells, Array2D<OutputCell>& output_cells, Vec2I origin, int range_limit, Function set_visible)
  {
    assert(input_cells.size() == output_cells.size());

    if (!output_cells.valid(origin)) {
      return;
    }

    OutputCell& cell = output_cells(origin);
    set_visible(cell);

    for (auto direction : { Direction::Up, Direction::Left, Direction::Down, Direction::Right }) {
      const details::Quadrant quadrant = { direction, origin };
      details::compute_visibility_in_quadrant(input_cells, output_cells, origin, range_limit, quadrant, set_visible);
    }
  }

}

#endif // GF_FIELD_OF_VISION_H
