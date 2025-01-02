// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_HEIGHTMAP_H
#define GF_HEIGHTMAP_H

#include <cstdint>

#include <tuple>

#include "Array2D.h"
#include "ColorRamp.h"
#include "CoreApi.h"
#include "Image.h"
#include "Noise.h"
#include "Rect.h"
#include "Vec2.h"

namespace gf {

  enum class HeightmapRender : uint8_t {
    Colored,
    Shaded,
  };

  class GF_CORE_API Heightmap {
  public:
    Heightmap() = default;
    Heightmap(Vec2I size);

    Vec2I size() const
    {
      return m_data.size();
    }

    void reset();

    double value(Vec2I position) const
    {
      return m_data(position);
    }

    void set_value(Vec2I position, double value)
    {
      m_data(position) = value;
    }

    std::tuple<double, double> get_min_max() const;

    void normalize(double min = 0.0, double max = 1.0);
    void add_hill(Vec2D center, double radius, double height);
    void dig_hill(Vec2D center, double radius, double height);
    void add_noise(Noise2D* noise, double scale = 1.0);
    void translate(double offset);
    void scale(double factor);
    void clamp(double min = 0.0, double max = 1.0);

    double slope(Vec2I position) const;
    void thermal_erosion(int iterations, double talus, double fraction);
    void hydraulic_erosion(int iterations, double rain_amount, double solubility, double evaporation, double capacity);
    void fast_erosion(int iterations, double talus, double fraction);
    double erosion_score() const;

    Heightmap sub_map(RectI area) const;
    Image copy_to_grayscale_image() const;

    Image copy_to_colored_image(const ColorRamp& ramp, double water_level = 0.5, HeightmapRender render = HeightmapRender::Colored) const;

  private:
    Array2D<double> m_data;
  };

}

#endif // GF_HEIGHTMAP_H
