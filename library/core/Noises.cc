// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/Noises.h>

#include <algorithm>

#include <gf2/core/Math.h>

namespace gf {

  namespace {

    void generate_permutation(Random& random, std::array<uint8_t, 256>& permutation)
    {
      std::iota(permutation.begin(), permutation.end(), 0);
      std::shuffle(permutation.begin(), permutation.end(), random.engine());
    }

  }

  /*
   * ValueNoise2D
   */

  ValueNoise2D::ValueNoise2D(Random& random, Step<double> step)
  : m_step(step)
  {
    // initialize permutation

    generate_permutation(random, m_permutation);

    // generate values

    std::uniform_real_distribution<double> distribution_value(0.0, 1.0);

    for (auto& value : m_values) {
      value = distribution_value(random.engine());
    }
  }

  double ValueNoise2D::value(double x, double y)
  {
    auto qx = static_cast<uint8_t>(std::fmod(x, 256));
    const double rx = std::fmod(x, 1);
    assert(rx >= 0.0 && rx <= 1.0);

    auto qy = static_cast<uint8_t>(std::fmod(y, 256));
    const double ry = std::fmod(y, 1);
    assert(ry >= 0.0 && ry <= 1.0);

    // clang-format off
    const double nw = at(qx    , qy    );
    const double ne = at(qx + 1, qy    );
    const double sw = at(qx    , qy + 1);
    const double se = at(qx + 1, qy + 1);
    // clang-format on

    const double n = gf::lerp(nw, ne, m_step(rx));
    const double s = gf::lerp(sw, se, m_step(rx));

    return gf::lerp(n, s, m_step(ry));
  }

  double ValueNoise2D::at(uint8_t i, uint8_t j) const
  {
    uint8_t index = i;
    index = m_permutation.at(index) + j;
    return m_values.at(m_permutation.at(index));
  }

  /*
   * GradientNoise2D
   */

  GradientNoise2D::GradientNoise2D(Random& random, Step<double> step)
  : m_step(step)
  {
    // initialize permutation

    generate_permutation(random, m_permutation);

    // generate 2D gradients

    std::uniform_real_distribution<double> distribution_angle(0.0, 2.0 * constants::Pi<double>);

    for (auto& gradient : m_gradients) {
      const double angle = distribution_angle(random.engine());
      gradient = gf::unit(angle);
    }
  }

  double GradientNoise2D::value(double x, double y)
  {
    auto qx = static_cast<uint8_t>(std::fmod(x, 256));
    const double rx = std::fmod(x, 1);
    assert(rx >= 0.0 && rx <= 1.0);

    auto qy = static_cast<uint8_t>(std::fmod(y, 256));
    const double ry = std::fmod(y, 1);
    assert(ry >= 0.0 && ry <= 1.0);

    // clang-format off
    const double p00 = dot(at(qx    , qy    ), {rx      , ry      });
    const double p10 = dot(at(qx + 1, qy    ), {rx - 1.0, ry      });
    const double p01 = dot(at(qx    , qy + 1), {rx      , ry - 1.0});
    const double p11 = dot(at(qx + 1, qy + 1), {rx - 1.0, ry - 1.0});
    // clang-format on

    const double u = m_step(rx);

    const double p0 = gf::lerp(p00, p10, u);
    const double p1 = gf::lerp(p01, p11, u);

    const double v = m_step(ry);

    return gf::lerp(p0, p1, v);
  }

  Vec2D GradientNoise2D::at(uint8_t i, uint8_t j) const
  {
    uint8_t index = i;
    index = m_permutation.at(index) + j;
    return m_gradients.at(m_permutation.at(index));
  }

  /*
   * GradientNoise3D
   */

  GradientNoise3D::GradientNoise3D(Random& random, Step<double> step)
  : m_step(step)
  {
    // initialize permutation

    generate_permutation(random, m_permutation);

    // generate 3D gradients

    std::uniform_real_distribution<double> distribution_phi(0.0, 2.0 * constants::Pi<double>);
    std::uniform_real_distribution<double> distribution_theta(0.0, 2.0 * constants::Pi<double>);

    for (auto& gradient : m_gradients) {
      const double phi = distribution_phi(random.engine());
      const double theta = distribution_theta(random.engine());
      gradient = { std::cos(phi) * std::sin(theta), std::sin(phi) * std::sin(theta), std::cos(theta) };
    }
  }

  double GradientNoise3D::value(double x, double y, double z)
  {
    auto qx = static_cast<uint8_t>(std::fmod(x, 256));
    const double rx = std::fmod(x, 1);
    assert(rx >= 0.0 && rx <= 1.0);

    auto qy = static_cast<uint8_t>(std::fmod(y, 256));
    const double ry = std::fmod(y, 1);
    assert(ry >= 0.0 && ry <= 1.0);

    auto qz = static_cast<uint8_t>(std::fmod(z, 256));
    const double rz = std::fmod(z, 1);
    assert(rz >= 0.0 && rz <= 1.0);

    // clang-format off
    const double p000 = dot(at(qx    , qy    , qz    ), {rx      , ry      , rz      });
    const double p100 = dot(at(qx + 1, qy    , qz    ), {rx - 1.0, ry      , rz      });
    const double p010 = dot(at(qx    , qy + 1, qz    ), {rx      , ry - 1.0, rz      });
    const double p110 = dot(at(qx + 1, qy + 1, qz    ), {rx - 1.0, ry - 1.0, rz      });
    const double p001 = dot(at(qx    , qy    , qz + 1), {rx      , ry      , rz - 1.0});
    const double p101 = dot(at(qx + 1, qy    , qz + 1), {rx - 1.0, ry      , rz - 1.0});
    const double p011 = dot(at(qx    , qy + 1, qz + 1), {rx      , ry - 1.0, rz - 1.0});
    const double p111 = dot(at(qx + 1, qy + 1, qz + 1), {rx - 1.0, ry - 1.0, rz - 1.0});
    // clang-format on

    const double u = m_step(rx);

    const double p00 = gf::lerp(p000, p100, u);
    const double p10 = gf::lerp(p010, p110, u);
    const double p01 = gf::lerp(p001, p101, u);
    const double p11 = gf::lerp(p011, p111, u);

    const double v = m_step(ry);

    const double p0 = gf::lerp(p00, p10, v);
    const double p1 = gf::lerp(p01, p11, v);

    const double w = m_step(rz);

    return gf::lerp(p0, p1, w);
  }

  /*
   * BetterGradientNoise2D
   */

  BetterGradientNoise2D::BetterGradientNoise2D(Random& random)
  {
    // initialize permutation

    generate_permutation(random, m_permutation_x);
    generate_permutation(random, m_permutation_y);

    // generate 2D gradients

    std::uniform_real_distribution<double> distribution_angle(0.0, 2.0 * constants::Pi<double>);

    for (auto& gradient : m_gradients) {
      const double angle = distribution_angle(random.engine());
      gradient = gf::unit(angle);
    }
  }

  double BetterGradientNoise2D::value(double x, double y)
  {
    auto qx = static_cast<uint8_t>(std::fmod(x, 256));
    const double rx = std::fmod(x, 1);
    assert(rx >= 0.0 && rx <= 1.0);

    auto qy = static_cast<uint8_t>(std::fmod(y, 256));
    const double ry = std::fmod(y, 1);
    assert(ry >= 0.0 && ry <= 1.0);

    double value = 0.0f;

    for (int j = -1; j <= 1; ++j) {
      for (int i = -1; i <= 1; ++i) {
        const Vec2D r(rx - i, ry - j);
        const double d = gf::square_length(r);

        if (d < 4) {
          const double t = 1 - d / 4;
          value += dot(at(static_cast<uint8_t>(qx + i), static_cast<uint8_t>(qy + j)), r) * t * t * t * t * (4 * t - 3);
        }
      }
    }

    return value;
  }

  Vec2D BetterGradientNoise2D::at(uint8_t i, uint8_t j) const
  {
    const uint8_t index = m_permutation_x.at(i) ^ m_permutation_y.at(j);
    return m_gradients.at(index);
  }

  /*
   * FractalNoise2D
   */

  FractalNoise2D::FractalNoise2D(Noise2D* noise, double scale, int octaves, double lacunarity, double persistence, double dimension)
  : m_noise(noise)
  , m_scale(scale)
  , m_octaves(octaves)
  , m_lacunarity(lacunarity)
  , m_persistence(persistence)
  , m_dimension(dimension)
  {
  }

  double FractalNoise2D::value(double x, double y)
  {
    if (m_noise == nullptr) {
      return 0.0;
    }

    double value = 0.0;
    double frequency = 1.0;
    double amplitude = 1.0;

    x *= m_scale;
    y *= m_scale;

    for (int k = 0; k < m_octaves; ++k) {
      value += m_noise->value(x * frequency, y * frequency) * std::pow(amplitude, m_dimension);

      frequency *= m_lacunarity;
      amplitude *= m_persistence;
    }

    return value;
  }

  /*
   * FractalNoise3D
   */

  FractalNoise3D::FractalNoise3D(Noise3D* noise, double scale, int octaves, double lacunarity, double persistence, double dimension)
  : m_noise(noise)
  , m_scale(scale)
  , m_octaves(octaves)
  , m_lacunarity(lacunarity)
  , m_persistence(persistence)
  , m_dimension(dimension)
  {
  }

  double FractalNoise3D::value(double x, double y, double z)
  {
    if (m_noise == nullptr) {
      return 0.0;
    }

    double value = 0.0;
    double frequency = 1.0;
    double amplitude = 1.0;

    x *= m_scale;
    y *= m_scale;
    z *= m_scale;

    for (int k = 0; k < m_octaves; ++k) {
      value += m_noise->value(x * frequency, y * frequency, z * frequency) * std::pow(amplitude, m_dimension);

      frequency *= m_lacunarity;
      amplitude *= m_persistence;
    }

    return value;
  }

  /*
   * PerlinNoise2D
   */

  PerlinNoise2D::PerlinNoise2D(Random& random, double scale, int octaves)
  : m_gradient_noise(random, gf::quintic_step)
  , m_fractal_noise(&m_gradient_noise, scale, octaves)
  {
  }

  double PerlinNoise2D::value(double x, double y)
  {
    return m_fractal_noise(x, y);
  }

  /*
   * PerlinNoise3D
   */

  PerlinNoise3D::PerlinNoise3D(Random& random, double scale, int octaves)
  : m_gradient_noise(random, gf::quintic_step)
  , m_fractal_noise(&m_gradient_noise, scale, octaves)
  {
  }

  double PerlinNoise3D::value(double x, double y, double z)
  {
    return m_fractal_noise(x, y, z);
  }

  /*
   * SimplexNoise2D
   */

  SimplexNoise2D::SimplexNoise2D(Random& random)
  {
    generate_permutation(random, m_permutation);
  }

  double SimplexNoise2D::value(double x, double y)
  {
    static constexpr double F2 = .366025403784438646763723170752; // (sqrt(3) - 1) / 2
    static constexpr double G2 = .211324865405187117745425609748; // K / (1 + 2 * K)

    const double s = (x + y) * F2;
    double i = x + s;
    double j = y + s;

    i = std::floor(i);
    j = std::floor(j);

    const double t = (i + j) * G2;
    double x0 = i - t;
    double y0 = j - t;

    x0 = x - x0;
    y0 = y - y0;

    uint8_t i1 = 0;
    uint8_t j1 = 0;

    if (x0 > y0) {
      i1 = 1;
    } else {
      j1 = 1;
    }

    const double x1 = x0 - i1 + G2;
    const double y1 = y0 - j1 + G2;

    const double x2 = x0 - 1 + 2 * G2;
    const double y2 = y0 - 1 + 2 * G2;

    auto ii = static_cast<uint8_t>(i);
    auto jj = static_cast<uint8_t>(j);

    double res = 0.0;

    double d0 = 0.5 - x0 * x0 - y0 * y0;

    if (d0 > 0) {
      d0 *= d0;
      res += d0 * d0 * dot(at(ii, jj), { x0, y0 });
    }

    double d1 = 0.5 - x1 * x1 - y1 * y1;

    if (d1 > 0) {
      d1 *= d1;
      res += d1 * d1 * dot(at(ii + i1, jj + j1), { x1, y1 });
    }

    double d2 = 0.5 - x2 * x2 - y2 * y2;

    if (d2 > 0) {
      d2 *= d2;
      res += d2 * d2 * dot(at(ii + 1, jj + 1), { x2, y2 });
    }

    return 45.23065 * res;
  }

  /*
   *         |
   *      1  -  0
   *         |
   *   6     -     4
   *         |
   * --|--|--+--|--|--
   *         |
   *   7     -     5
   *         |
   *      3  -  2
   *         |
   */

  Vec2D SimplexNoise2D::at(uint8_t i, uint8_t j) const
  {
    // clang-format off
    static constexpr std::array<Vec2D, 8> Gradients = {
      gf::vec( 1.0,  2.0),
      gf::vec(-1.0,  2.0),
      gf::vec( 1.0, -2.0),
      gf::vec(-1.0, -2.0),
      gf::vec( 2.0,  1.0),
      gf::vec( 2.0, -1.0),
      gf::vec(-2.0,  1.0),
      gf::vec(-2.0, -1.0)
    };
    // clang-format on

    uint8_t index = i;
    index = m_permutation.at(index) + j;
    return Gradients.at(m_permutation.at(index) % 8);
  }

}
