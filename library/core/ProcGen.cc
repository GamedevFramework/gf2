// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/ProcGen.h>

namespace gf {

  /*
   * Midpoint Displacement 1D
   */

  GF_CORE_API std::vector<Vec2F> midpoint_displacement_1d(Vec2F p0, Vec2F p1, Random& random, int iterations, Vec2F direction, float initial_factor, float reduction_factor)
  {
    direction = initial_factor * gf::euclidean_distance(p0, p1) * gf::normalize(direction);

    std::vector<Vec2F> points;

    const std::size_t size = static_cast<std::size_t>(1) << iterations;
    const std::size_t count = size + 1;
    points.resize(count);
    points[0] = p0;
    points[count - 1] = p1;

    std::size_t step = size / 2;

    while (step > 0) {
      for (std::size_t i = step; i < size; i += 2 * step) {
        assert(i - step < count);
        const Vec2F prev = points[i - step];
        assert(i + step < count);
        const Vec2F next = points[i + step];

        Vec2F middle = (prev + next) / 2;
        middle += random.compute_uniform_float(-0.5f, +0.5f) * direction;

        points[i] = middle;
      }

      direction *= reduction_factor;
      step /= 2;
    }

    return points;
  }

  GF_CORE_API std::vector<Vec2F> midpoint_displacement_1d(Vec2F p0, Vec2F p1, Random& random, int iterations, float initial_factor, float reduction_factor)
  {
    return midpoint_displacement_1d(p0, p1, random, iterations, gf::perp(p1 - p0), initial_factor, reduction_factor);
  }

  /*
   * Midpoint Displacement 2D
   */

  namespace {

    int compute_power_of_two_size(Vec2I size)
    {
      int actual_size = 1;

      while (actual_size + 1 < size.h || actual_size + 1 < size.w) {
        actual_size = actual_size * 2;
      }

      return actual_size;
    }

    void initialize_corners(Heightmap& map, Span<const double> initial_values, int d)
    {
      if (initial_values.empty()) {
        map.set_value({ 0, 0 }, 0.0);
        map.set_value({ 0, d }, 0.0);
        map.set_value({ d, d }, 0.0);
        map.set_value({ d, 0 }, 0.0);
      } else if (initial_values.size() < 4) {
        map.set_value({ 0, 0 }, initial_values[0]);
        map.set_value({ 0, d }, initial_values[0]);
        map.set_value({ d, d }, initial_values[0]);
        map.set_value({ d, 0 }, initial_values[0]);
      } else {
        map.set_value({ 0, 0 }, initial_values[0]);
        map.set_value({ 0, d }, initial_values[1]);
        map.set_value({ d, d }, initial_values[2]);
        map.set_value({ d, 0 }, initial_values[3]);
      }
    }

  } // anonymous namespace

  GF_CORE_API Heightmap midpoint_displacement_2d(Vec2I size, Random& random, Span<const double> initial_values)
  {
    int actual_size = compute_power_of_two_size(size);

    int d = actual_size;
    actual_size = actual_size + 1;

    Heightmap map({ actual_size, actual_size });
    initialize_corners(map, initial_values, d);

    while (d >= 2) {
      const int d2 = d / 2;

      std::uniform_real_distribution<double> dist(-static_cast<double>(d), static_cast<double>(d));

      for (int y = d2; y < actual_size; y += d) {
        for (int x = d2; x < actual_size; x += d) {
          const double ne = map.value({ x - d2, y - d2 });
          const double nw = map.value({ x - d2, y + d2 });
          const double se = map.value({ x + d2, y - d2 });
          const double sw = map.value({ x + d2, y + d2 });

          // center
          const double center = (ne + nw + se + sw) / 4;
          map.set_value({ x, y }, center + dist(random.engine()));

          // north
          const double north = (ne + nw) / 2;
          map.set_value({ x - d2, y }, north + dist(random.engine()));

          // south
          const double south = (se + sw) / 2;
          map.set_value({ x + d2, y }, south + dist(random.engine()));

          // east
          const double east = (ne + se) / 2;
          map.set_value({ x, y - d2 }, east + dist(random.engine()));

          // west
          const double west = (nw + sw) / 2;
          map.set_value({ x, y + d2 }, west + dist(random.engine()));
        }
      }

      d = d2;
    }

    const Vec2I offset = (actual_size - size) / 2;
    return map.sub_map(RectI::from_position_size(offset, size));
  }

  /*
   * Diamond-Square
   */

  namespace {

    void diamond(Heightmap& map, Random& random, Vec2I pos, int d)
    {
      // clang-format off
      const double value = (map.value({ pos.x - d, pos.y - d })
                          + map.value({ pos.x - d, pos.y + d })
                          + map.value({ pos.x + d, pos.y - d })
                          + map.value({ pos.x + d, pos.y + d })) / 4;
      // clang-format on

      const double noise = random.compute_uniform_float(-static_cast<double>(d), +static_cast<double>(d));
      map.set_value(pos, value + noise);
    }

    void square(Heightmap& map, Random& random, Vec2I pos, int d)
    {
      const Vec2I size = map.size();

      double value = 0.0;
      int n = 0;

      if (pos.x >= d) {
        value += map.value({ pos.x - d, pos.y });
        ++n;
      }

      if (pos.x + d < size.w) {
        value += map.value({ pos.x + d, pos.y });
        ++n;
      }

      if (pos.y >= d) {
        value += map.value({ pos.x, pos.y - d });
        ++n;
      }

      if (pos.y + d < size.h) {
        value += map.value({ pos.x, pos.y + d });
        ++n;
      }

      assert(n > 0);
      value = value / n;

      const double noise = random.compute_uniform_float(-static_cast<double>(d), static_cast<double>(d));
      map.set_value(pos, value + noise);
    }

  } // anonymous namespace

  GF_CORE_API Heightmap diamond_square_2d(Vec2I size, Random& random, Span<const double> initial_values)
  {
    int actual_size = compute_power_of_two_size(size);

    int d = actual_size;
    actual_size = actual_size + 1;

    Heightmap map({ actual_size, actual_size });
    initialize_corners(map, initial_values, d);

    while (d >= 2) {
      const int d2 = d / 2;

      for (int y = d2; y < actual_size; y += d) {
        for (int x = d2; x < actual_size; x += d) {
          diamond(map, random, { x, y }, d2);
        }
      }

      for (int y = 0; y < actual_size; y += d) {
        for (int x = d2; x < actual_size; x += d) {
          square(map, random, { x, y }, d2);
        }
      }

      for (int y = d2; y < actual_size; y += d) {
        for (int x = 0; x < actual_size; x += d) {
          square(map, random, { x, y }, d2);
        }
      }

      d = d2;
    }

    const Vec2I offset = (actual_size - size) / 2;
    return map.sub_map(RectI::from_position_size(offset, size));
  }

}
