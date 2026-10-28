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

    float value(Vec2I position) const
    {
      return m_data(position);
    }

    void set_value(Vec2I position, float value)
    {
      m_data(position) = value;
    }

    std::tuple<float, float> get_min_max() const;

    void normalize(float min = 0.0, float max = 1.0);
    void add_hill(Vec2F center, float radius, float height);
    void dig_hill(Vec2F center, float radius, float height);
    void add_noise(Noise2D* noise, float scale = 1.0);
    void translate(float offset);
    void scale(float factor);
    void clamp(float min = 0.0, float max = 1.0);

    float slope(Vec2I position) const;
    void thermal_erosion(int iterations, float talus, float fraction);
    void hydraulic_erosion(int iterations, float rain_amount, float solubility, float evaporation, float capacity);
    void fast_erosion(int iterations, float talus, float fraction);
    float erosion_score() const;

    Heightmap sub_map(RectI area) const;
    Image copy_to_grayscale_image() const;

    Image copy_to_colored_image(const ColorRamp& ramp, float water_level = 0.5f, HeightmapRender render = HeightmapRender::Colored) const;

  private:
    Array2D<float> m_data;
  };

}

#endif // GF_HEIGHTMAP_H
