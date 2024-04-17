// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/Heightmap.h>

#include <algorithm>

#include <gf2/core/Range.h>
#include <gf2/core/Vec3.h>

namespace gf {

  Heightmap::Heightmap(Vec2I size)
  : m_data(size, 0.0)
  {
  }

  void Heightmap::reset()
  {
    std::fill(m_data.begin(), m_data.end(), 0.0);
  }

  std::tuple<double, double> Heightmap::get_min_max() const
  {
    auto p = std::minmax_element(m_data.begin(), m_data.end());
    return std::make_tuple(*p.first, *p.second);
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
      value = min + (value - current_min) * factor;
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
            if (m_data({ x, y }) < z) {
              m_data({ x, y }) = z;
            }
          } else {
            if (m_data({ x, y }) > z) {
              m_data({ x, y }) = z;
            }
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
      Vec2D location = Vec2D(position) / m_data.size() * scale;
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
      double altitude_difference = std::abs(altitude - m_data(neighbor));

      if (altitude_difference > altitude_difference_max) {
        altitude_difference_max = altitude_difference;
      }
    }

    return altitude_difference_max;
  }

  void Heightmap::thermal_erosion(int iterations, double talus, double fraction)
  {
    double d[3][3];

    Array2D<double> material(m_data.size());

    auto size = m_data.size();

    for (int k = 0; k < iterations; ++k) {

      // initialize material map
      std::fill(material.begin(), material.end(), 0.0);

      // compute material map
      for (int y = 1; y < size.h - 1; ++y) {
        for (int x = 1; x < size.w - 1; ++x) {
          double diffTotal = 0.0;
          double diffMax = 0.0;

          for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
              double diff = m_data({ x, y }) - m_data({ x + i, y + j });
              d[1 + i][1 + j] = diff;

              if (diff > talus) {
                diffTotal += diff;

                if (diff > diffMax) {
                  diffMax = diff;
                }
              }
            }
          }

          for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
              double diff = d[1 + i][1 + j];

              if (diff > talus) {
                material({ x + i, y + j }) += fraction * (diffMax - talus) * (diff / diffTotal);
              }
            }
          }
        }
      }

      // add material map to the heightmap
      for (int y = 1; y < size.h - 1; ++y) {
        for (int x = 1; x < size.w - 1; ++x) {
          m_data({ x, y }) += material({ x, y });
        }
      }
    }
  }

  void Heightmap::hydraulic_erosion(int iterations, double rain_amount, double solubility, double evaporation, double capacity)
  {
    Array2D<double> waterMap(m_data.size(), 0.0);
    Array2D<double> waterDiff(m_data.size(), 0.0);

    Array2D<double> materialMap(m_data.size(), 0.0);
    Array2D<double> materialDiff(m_data.size(), 0.0);

    double d[3][3];

    auto size = m_data.size();

    for (int k = 0; k < iterations; ++k) {

      // 1. appearance of new water
      for (auto& water : waterMap) {
        water += rain_amount;
      }

      // 2. water erosion of the terrain
      for (auto pos : waterMap.position_range()) {
        double material = solubility * waterMap(pos);
        m_data(pos) -= material;
        materialMap(pos) += material;
      }

      // 3. transportation of water
      std::fill(waterDiff.begin(), waterDiff.end(), 0.0);
      std::fill(materialDiff.begin(), materialDiff.end(), 0.0);

      for (int y = 1; y < size.h - 1; ++y) {
        for (int x = 1; x < size.w - 1; ++x) {
          double diffTotal = 0.0;
          double altitudeTotal = 0.0;
          double altitude = m_data({ x, y }) + waterMap({ x, y });
          int n = 0;

          for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
              double altitudeLocal = m_data({ x + i, y + j }) + waterMap({ x + i, y + j });
              double diff = altitude - altitudeLocal;
              d[1 + i][1 + j] = diff;

              if (diff > 0.0) {
                diffTotal += diff;
                altitudeTotal += altitudeLocal;
                n++;
              }
            }
          }

          if (n == 0) {
            continue;
          }

          double altitudeAverage = altitudeTotal / n;
          double diffAltitude = std::min(waterMap({ x, y }), altitude - altitudeAverage);

          for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
              double diff = d[1 + i][1 + j];

              if (diff > 0.0) {
                double diffWater = diffAltitude * (diff / diffTotal);
                waterDiff({ x + i, y + j }) += diffWater;
                waterDiff({ x, y }) -= diffWater;

                double diffMaterial = materialMap({ x, y }) * (diffWater / waterMap({ x, y }));
                materialDiff({ x + i, y + j }) += diffMaterial;
                materialDiff({ x, y }) -= diffMaterial;
              }
            }
          }
        }
      }

      for (auto pos : waterMap.position_range()) {
        waterMap(pos) += waterDiff(pos);
      }

      for (auto pos : materialMap.position_range()) {
        materialMap(pos) += materialDiff(pos);
      }

      // 4. evaporation of water
      for (auto pos : waterMap.position_range()) {
        double water = waterMap(pos) * (1 - evaporation);

        waterMap(pos) = water;

        double materialMax = capacity * water;
        double diffMaterial = std::max(double(0), materialMap(pos) - materialMax);
        materialMap(pos) -= diffMaterial;
        m_data(pos) += diffMaterial;
      }
    }
  }

  void Heightmap::fast_erosion(int iterations, double talus, double fraction)
  {
    Array2D<double> material(m_data.size());

    for (int k = 0; k < iterations; ++k) {

      // initialize material map
      std::fill(material.begin(), material.end(), 0.0);

      // compute material map
      for (auto position : m_data.position_range()) {
        double altitudeDifferenceMax = 0.0;
        Vec2I positionMax = position;

        const double altitude = m_data(position);

        for (auto positionThere : m_data.compute_8_neighbors_range(position)) {
          double altitudeThere = m_data(positionThere);

          double altitudeDifference = altitude - altitudeThere;
          if (altitudeDifference > altitudeDifferenceMax) {
            altitudeDifferenceMax = altitudeDifference;
            positionMax = positionThere;
          }
        }

        if (0 < altitudeDifferenceMax && altitudeDifferenceMax <= talus) {
          material(position) -= fraction * altitudeDifferenceMax;
          material(positionMax) += fraction * altitudeDifferenceMax;
        }
      }

      // add material map to the map
      for (auto position : m_data.position_range()) {
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

    RectI intersection = *maybe_intersection;
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

      return (value - water_level) / (1.0 - water_level) * 0.5 + 0.5;
    }

  } // anonymous namespace

  Image Heightmap::copy_to_colored_image(const ColorRamp& ramp, double water_level, HeightmapRender render) const
  {
    auto size = m_data.size();
    Image image(size);

    for (auto pos : m_data.position_range()) {
      double value = value_with_water_level(m_data(pos), water_level);
      Color color = ramp.compute_color(static_cast<float>(value));
      image.put_pixel(pos, color);
    }

    if (render == HeightmapRender::Shaded) {
      static constexpr Vec3D Light = { -1.0, -1.0, 0.0 };

      for (auto pos : m_data.position_range()) {
        if (m_data(pos) < water_level) {
          continue;
        }

        double x = pos.x;
        double y = pos.y;

        // compute the normal vector
        Vec3D normal(0, 0, 0);
        int count = 0;

        Vec3D p{ x, y, m_data(pos) };

        if (pos.x > 0 && pos.y > 0) {
          Vec3D pn{ x, y - 1, m_data({ pos.x, pos.y - 1 }) };
          Vec3D pw{ x - 1, y, m_data({ pos.x - 1, pos.y }) };

          Vec3D v3 = cross(p - pw, p - pn);
          assert(v3.z > 0);

          normal += v3;
          count += 1;
        }

        if (pos.x > 0 && pos.y < size.h - 1) {
          Vec3D pw{ x - 1, y, m_data({ pos.x - 1, pos.y }) };
          Vec3D ps{ x, y + 1, m_data({ pos.x, pos.y + 1 }) };

          Vec3D v3 = cross(p - ps, p - pw);
          assert(v3.z > 0);

          normal += v3;
          count += 1;
        }

        if (pos.x < size.w - 1 && pos.y > 0) {
          Vec3D pe{ x + 1, y, m_data({ pos.x + 1, pos.y }) };
          Vec3D pn{ x, y - 1, m_data({ pos.x, pos.y - 1 }) };

          Vec3D v3 = cross(p - pn, p - pe);
          assert(v3.z > 0);

          normal += v3;
          count += 1;
        }

        if (pos.x < size.w - 1 && pos.y < size.h - 1) {
          Vec3D pe{ x + 1, y, m_data({ pos.x + 1, pos.y }) };
          Vec3D ps{ x, y + 1, m_data({ pos.x, pos.y + 1 }) };

          Vec3D v3 = cross(p - pe, p - ps);
          assert(v3.z > 0);

          normal += v3;
          count += 1;
        }

        normal = gf::normalize(normal / count);
        double d = gf::dot(Light, normal);
        d = gf::clamp(0.5 + 35 * d, 0.0, 1.0);

        Color pixel = image(pos);

        Color lo = gf::lerp(pixel, Color(0x331133), 0.7);
        Color hi = gf::lerp(pixel, Color(0xFFFFCC), 0.3);

        if (d < 0.5) {
          image.put_pixel(pos, gf::lerp(lo, pixel, 2 * d));
        } else {
          image.put_pixel(pos, gf::lerp(pixel, hi, 2 * d - 1));
        }
      }
    }

    return image;
  }

}
