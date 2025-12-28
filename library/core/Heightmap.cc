// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Heightmap.h>

#include <algorithm>

#include <gf2/core/Direction.h>
#include <gf2/core/Range.h>
#include <gf2/core/Vec3.h>

namespace gf {

  Heightmap::Heightmap(Vec2I size)
  : m_data(size, 0.0)
  {
  }

  void Heightmap::reset()
  {
    std::ranges::fill(m_data, 0.0);
  }

  std::tuple<double, double> Heightmap::get_min_max() const
  {
    auto [ pmin, pmax ] = std::ranges::minmax_element(m_data);
    return std::make_tuple(*pmin, *pmax);
  }

  void Heightmap::normalize(double min, double max)
  {
    if (min > max) {
      std::swap(min, max);
    }

    auto [current_min, current_max] = get_min_max();

    double factor = 0.0;

    if (!gf::almost_equals(current_min, current_max)) {
      factor = (max - min) / (current_max - current_min);
    }

    for (auto& value : m_data) {
      value = min + ((value - current_min) * factor);
    }
  }

  void Heightmap::add_hill(Vec2D center, double radius, double height)
  {
    const Vec2I size = m_data.size();
    const double radius_square = gf::square(radius);
    const double factor = height / radius_square;
    const int min_x = std::max(0, static_cast<int>(center.x - radius));
    const int max_x = std::min(size.w, static_cast<int>(center.x + radius));
    const int min_y = std::max(0, static_cast<int>(center.y - radius));
    const int max_y = std::min(size.h, static_cast<int>(center.y + radius));

    for (int y = min_y; y < max_y; ++y) {
      const double y_dist_square = gf::square(y - center.y);

      for (int x = min_x; x < max_x; ++x) {
        const double x_dist_square = gf::square(x - center.x);
        const double z = radius_square - (y_dist_square + x_dist_square);

        if (z > 0.0) {
          m_data({ x, y }) += z * factor;
        }
      }
    }
  }

  void Heightmap::dig_hill(Vec2D center, double radius, double height)
  {
    const Vec2I size = m_data.size();
    const double radius_square = gf::square(radius);
    const double factor = height / radius_square;
    const int min_x = std::max(0, static_cast<int>(center.x - radius));
    const int max_x = std::min(size.w, static_cast<int>(center.x + radius));
    const int min_y = std::max(0, static_cast<int>(center.y - radius));
    const int max_y = std::min(size.h, static_cast<int>(center.y + radius));

    for (int y = min_y; y < max_y; ++y) {
      const double y_dist_square = gf::square(y - center.y);

      for (int x = min_x; x < max_x; ++x) {
        const double x_dist_square = gf::square(x - center.x);
        const double dist_square = y_dist_square + x_dist_square;

        if (dist_square < radius_square) {
          const double z = (radius_square - dist_square) * factor;

          if (height > 0.0) {
            m_data({ x, y }) = std::max(m_data({ x, y }), z);
          } else {
            m_data({ x, y }) = std::min(m_data({ x, y }), z);
          }
        }
      }
    }
  }

  void Heightmap::add_noise(Noise2D* noise, double scale)
  {
    if (noise == nullptr) {
      return;
    }

    for (auto position : m_data.position_range()) {
      const Vec2D location = Vec2D(position) / m_data.size() * scale;
      m_data(position) += noise->value(location.x, location.y);
    }
  }

  void Heightmap::translate(double offset)
  {
    for (auto& current_value : m_data) {
      current_value += offset;
    }
  }

  void Heightmap::scale(double factor)
  {
    for (auto& current_value : m_data) {
      current_value *= factor;
    }
  }

  void Heightmap::clamp(double min, double max)
  {
    for (auto& value : m_data) {
      value = gf::clamp(value, min, max);
    }
  }

  double Heightmap::slope(Vec2I position) const
  {
    const double altitude = m_data(position);
    double altitude_difference_max = 0.0;

    for (auto neighbor : m_data.compute_4_neighbors_range(position)) {
      const double altitude_difference = std::abs(altitude - m_data(neighbor));
      altitude_difference_max = std::max(altitude_difference, altitude_difference_max);
    }

    return altitude_difference_max;
  }

  void Heightmap::thermal_erosion(int iterations, double talus, double fraction)
  {
    Array2D<double> material(m_data.size());
    const RectI outer = RectI::from_size(m_data.size()).shrink_by(1);
    const RectI inner = RectI::from_position_size({ -1, -1 }, { 3, 3 });

    for (int k = 0; k < iterations; ++k) {
      // initialize material map
      std::ranges::fill(material, 0.0);

      // compute material map
      for (auto position : rectangle_range(outer)) {
        double difference_total = 0.0;
        double difference_max = 0.0;
        double difference_array[3][3] = {};

        for (auto displacement : rectangle_range(inner)) {
          const double difference = m_data(position) - m_data(position + displacement);
          difference_array[1 + displacement.x][1 + displacement.y] = difference;

          if (difference > talus) {
            difference_total += difference;
            difference_max = std::max(difference, difference_max);
          }
        }

        for (auto displacement : rectangle_range(inner)) {
          const double difference = difference_array[1 + displacement.x][1 + displacement.y];

          if (difference > talus) {
            material(position + displacement) += fraction * (difference_max - talus) * (difference / difference_total);
          }
        }
      }

      // add material map to the heightmap
      for (auto position : rectangle_range(outer)) {
        m_data(position) += material(position);
      }
    }
  }

  // NOLINTNEXTLINE(readability-function-cognitive-complexity)
  void Heightmap::hydraulic_erosion(int iterations, double rain_amount, double solubility, double evaporation, double capacity)
  {
    Array2D<double> water_map(m_data.size(), 0.0);
    Array2D<double> water_difference(m_data.size(), 0.0);

    Array2D<double> material_map(m_data.size(), 0.0);
    Array2D<double> material_difference(m_data.size(), 0.0);

    const RectI outer = RectI::from_size(m_data.size()).shrink_by(1);
    const RectI inner = RectI::from_position_size({ -1, -1 }, { 3, 3 });

    for (int k = 0; k < iterations; ++k) {
      // 1. appearance of new water
      for (double& water : water_map) {
        water += rain_amount;
      }

      // 2. water erosion of the terrain
      for (const Vec2I position : water_map.position_range()) {
        const double material = solubility * water_map(position);
        m_data(position) -= material;
        material_map(position) += material;
      }

      // 3. transportation of water
      std::ranges::fill(water_difference, 0.0);
      std::ranges::fill(material_difference, 0.0);

      for (const Vec2I position : rectangle_range(outer)) {
        double altitude_difference_total = 0.0;
        double altitude_total = 0.0;
        const double altitude = m_data(position) + water_map(position);
        double difference_array[3][3];
        int count = 0;

        for (auto displacement : rectangle_range(inner)) {
          const double altitude_neighbor = m_data(position + displacement) + water_map(position + displacement);
          const double altitude_difference = altitude - altitude_neighbor;
          difference_array[1 + displacement.x][1 + displacement.y] = altitude_difference;

          if (altitude_difference > 0.0) {
            altitude_difference_total += altitude_difference;
            altitude_total += altitude_neighbor;
            ++count;
          }
        }

        if (count == 0) {
          continue;
        }

        const double altitude_average = altitude_total / count;
        const double altitude_relative = std::min(water_map(position), altitude - altitude_average);

        for (const Vec2I displacement : rectangle_range(inner)) {
          const double altitude_difference = difference_array[1 + displacement.x][1 + displacement.y];

          if (altitude_difference > 0.0) {
            const double water = altitude_relative * (altitude_difference / altitude_difference_total);
            water_difference(position + displacement) += water;
            water_difference(position) -= water;

            const double material = material_map(position) * (water / water_map(position));
            material_difference(position + displacement) += material;
            material_difference(position) -= material;
          }
        }
      }

      for (const Vec2I position : m_data.position_range()) {
        water_map(position) += water_difference(position);
        material_map(position) += material_difference(position);
      }

      // 4. evaporation of water
      for (const Vec2I position : water_map.position_range()) {
        const double water = water_map(position) * (1 - evaporation);
        water_map(position) = water;

        const double material_max = capacity * water;
        const double material = std::max(0.0, material_map(position) - material_max);
        material_map(position) -= material;
        m_data(position) += material;
      }
    }
  }

  void Heightmap::fast_erosion(int iterations, double talus, double fraction)
  {
    Array2D<double> material(m_data.size());

    for (int k = 0; k < iterations; ++k) {

      // initialize material map
      std::ranges::fill(material, 0.0);

      // compute material map
      for (const Vec2I position : m_data.position_range()) {
        double altitude_difference_max = 0.0;
        Vec2I position_max = position;

        const double altitude = m_data(position);

        for (const Vec2I neighbor : m_data.compute_8_neighbors_range(position)) {
          const double altitude_neighbor = m_data(neighbor);
          const double altitude_difference = altitude - altitude_neighbor;

          if (altitude_difference > altitude_difference_max) {
            altitude_difference_max = altitude_difference;
            position_max = neighbor;
          }
        }

        if (0 < altitude_difference_max && altitude_difference_max <= talus) {
          material(position) -= fraction * altitude_difference_max;
          material(position_max) += fraction * altitude_difference_max;
        }
      }

      // add material map to the map
      for (const Vec2I position : m_data.position_range()) {
        m_data(position) += material(position);
      }
    }
  }

  double Heightmap::erosion_score() const
  {
    double total = 0.0;
    double total_square = 0.0;
    int n = 0;

    for (auto position : m_data.position_range()) {
      const double value = slope(position);
      total += value;
      total_square += gf::square(value);
      n++;
    }

    const double average = total / n;
    const double average_square = total_square / n;
    const double standard_deviation = std::sqrt(average_square - gf::square(average));
    return standard_deviation / average;
  }

  Heightmap Heightmap::sub_map(RectI area) const
  {
    auto maybe_intersection = RectI::from_size(m_data.size()).intersection(area);

    if (!maybe_intersection) {
      return {};
    }

    const RectI intersection = *maybe_intersection;
    Heightmap heightmap(intersection.size());

    for (auto position : gf::position_range(intersection.extent)) {
      heightmap.m_data(position) = m_data(position + intersection.offset);
    }

    return heightmap;
  }

  Image Heightmap::copy_to_grayscale_image() const
  {
    Image image(m_data.size());

    for (auto position : m_data.position_range()) {
      auto value = static_cast<float>(m_data(position) * 255.0);
      image.put_pixel(position, { value, value, value });
    }

    return image;
  }

  namespace {

    double value_with_water_level(double value, double water_level)
    {
      if (value < water_level) {
        return value / water_level * 0.5;
      }

      return ((value - water_level) / (1.0 - water_level) * 0.5) + 0.5;
    }

  } // anonymous namespace

  Image Heightmap::copy_to_colored_image(const ColorRamp& ramp, double water_level, HeightmapRender render) const
  {
    auto size = m_data.size();
    Image image(size);

    for (auto pos : m_data.position_range()) {
      const double value = value_with_water_level(m_data(pos), water_level);
      const Color color = ramp.compute_color(static_cast<float>(value));
      image.put_pixel(pos, color);
    }

    if (render == HeightmapRender::Shaded) {
      static constexpr Vec3D Light = { -1.0, -1.0, 0.0 };

      for (auto position : m_data.position_range()) {
        if (m_data(position) < water_level) {
          continue;
        }

        Vec3D normal(0.0, 0.0, 0.0);
        int count = 0;

        const Vec3D origin(position.x, position.y, m_data(position));

        for (auto direction : { Direction::Up, Direction::Right, Direction::Down, Direction::Left }) {
          const Vec2I direction0 = displacement(direction);
          const Vec2I direction1 = perp(direction0);

          if (m_data.valid(position + direction0) && m_data.valid(position + direction1)) {
            const Vec3D leaning0(position.x + direction0.x, position.y + direction0.y, m_data(position + direction0));
            const Vec3D leaning1(position.x + direction1.x, position.y + direction1.y, m_data(position + direction1));

            const Vec3D vertical = cross(origin - leaning0, origin - leaning1);
            assert(vertical.z > 0.0);

            normal += vertical;
            count += 1;
          }
        }

        normal = gf::normalize(normal / count);
        const double light = gf::clamp(0.5 + (35 * gf::dot(Light, normal)), 0.0, 1.0);

        const Color pixel = image(position);

        const Color lo = gf::lerp(pixel, Color(0x331133), 0.7f);
        const Color hi = gf::lerp(pixel, Color(0xFFFFCC), 0.3f);

        if (light < 0.5) {
          image.put_pixel(position, gf::lerp(lo, pixel, static_cast<float>(2 * light)));
        } else {
          image.put_pixel(position, gf::lerp(pixel, hi, static_cast<float>((2 * light) - 1)));
        }
      }
    }

    return image;
  }

}
