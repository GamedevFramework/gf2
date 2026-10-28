// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Noises.h>

#include <algorithm>
#include <iterator>
#include <numeric>

#include <gf2/core/Math.h>

namespace gf {

  namespace {

    void generate_permutation(Random* random, std::array<uint8_t, 256>& permutation)
    {
      std::iota(permutation.begin(), permutation.end(), 0);
      std::shuffle(permutation.begin(), permutation.end(), random->engine());
    }

  }

  /*
   * ValueNoise2D
   */

  ValueNoise2D::ValueNoise2D(Random* random, Step<float> step)
  : m_step(step)
  {
    // initialize permutation

    generate_permutation(random, m_permutation);

    // generate values

    for (float& value : m_values) {
      value = random->compute_uniform_float<float>();
    }
  }

  float ValueNoise2D::value(float x, float y)
  {
    float dummy = 0.0f;

    auto qx = static_cast<uint8_t>(x);
    const float rx = std::modf(x, &dummy);
    assert(rx >= 0.0f && rx <= 1.0f);

    auto qy = static_cast<uint8_t>(y);
    const float ry = std::modf(y, &dummy);
    assert(ry >= 0.0f && ry <= 1.0f);

    // clang-format off
    const float nw = at(qx    , qy    );
    const float ne = at(qx + 1, qy    );
    const float sw = at(qx    , qy + 1);
    const float se = at(qx + 1, qy + 1);
    // clang-format on

    const float u = m_step(rx);

    const float n = gf::lerp(nw, ne, u);
    const float s = gf::lerp(sw, se, u);

    const float v = m_step(ry);

    return gf::lerp(n, s, v);
  }

  float ValueNoise2D::at(uint8_t i, uint8_t j) const
  {
    uint8_t index = i;
    index = m_permutation.at(index) + j;
    return m_values.at(m_permutation.at(index));
  }

  /*
   * GradientNoise2D
   */

  GradientNoise2D::GradientNoise2D(Random* random, Step<float> step)
  : m_step(step)
  {
    // initialize permutation

    generate_permutation(random, m_permutation);

    // generate 2D gradients

    for (Vec2F& gradient : m_gradients) {
      const float angle = random->compute_uniform_float(2.0f * constants::Pi<float>);
      gradient = gf::unit(angle);
    }
  }

  float GradientNoise2D::value(float x, float y)
  {
    float dummy = 0.0f;

    auto qx = static_cast<uint8_t>(x);
    const float rx = std::modf(x, &dummy);
    assert(rx >= 0.0f && rx <= 1.0f);

    auto qy = static_cast<uint8_t>(y);
    const float ry = std::modf(y, &dummy);
    assert(ry >= 0.0f && ry <= 1.0f);

    // clang-format off
    const float p00 = dot(at(qx    , qy    ), {rx       , ry      });
    const float p10 = dot(at(qx + 1, qy    ), {rx - 1.0f, ry      });
    const float p01 = dot(at(qx    , qy + 1), {rx       , ry - 1.0f});
    const float p11 = dot(at(qx + 1, qy + 1), {rx - 1.0f, ry - 1.0f});
    // clang-format on

    const float u = m_step(rx);

    const float p0 = gf::lerp(p00, p10, u);
    const float p1 = gf::lerp(p01, p11, u);

    const float v = m_step(ry);

    return gf::lerp(p0, p1, v);
  }

  Vec2F GradientNoise2D::at(uint8_t i, uint8_t j) const
  {
    uint8_t index = i;
    index = m_permutation.at(index) + j;
    return m_gradients.at(m_permutation.at(index));
  }

  /*
   * GradientNoise3D
   */

  GradientNoise3D::GradientNoise3D(Random* random, Step<float> step)
  : m_step(step)
  {
    // initialize permutation

    generate_permutation(random, m_permutation);

    // generate 3D gradients

    for (Vec3F& gradient : m_gradients) {
      const float phi = random->compute_uniform_float(2.0f * constants::Pi<float>);
      const float theta = random->compute_uniform_float(2.0f * constants::Pi<float>);
      gradient = { std::cos(phi) * std::sin(theta), std::sin(phi) * std::sin(theta), std::cos(theta) };
    }
  }

  float GradientNoise3D::value(float x, float y, float z)
  {
    float dummy = 0.0f;

    auto qx = static_cast<uint8_t>(x);
    const float rx = std::modf(x, &dummy);
    assert(rx >= 0.0f && rx <= 1.0f);

    auto qy = static_cast<uint8_t>(y);
    const float ry = std::modf(y, &dummy);
    assert(ry >= 0.0f && ry <= 1.0f);

    auto qz = static_cast<uint8_t>(z);
    const float rz = std::modf(z, &dummy);
    assert(rz >= 0.0f && rz <= 1.0f);

    // clang-format off
    const float p000 = dot(at(qx    , qy    , qz    ), {rx       , ry       , rz       });
    const float p100 = dot(at(qx + 1, qy    , qz    ), {rx - 1.0f, ry       , rz       });
    const float p010 = dot(at(qx    , qy + 1, qz    ), {rx       , ry - 1.0f, rz       });
    const float p110 = dot(at(qx + 1, qy + 1, qz    ), {rx - 1.0f, ry - 1.0f, rz       });
    const float p001 = dot(at(qx    , qy    , qz + 1), {rx       , ry       , rz - 1.0f});
    const float p101 = dot(at(qx + 1, qy    , qz + 1), {rx - 1.0f, ry       , rz - 1.0f});
    const float p011 = dot(at(qx    , qy + 1, qz + 1), {rx       , ry - 1.0f, rz - 1.0f});
    const float p111 = dot(at(qx + 1, qy + 1, qz + 1), {rx - 1.0f, ry - 1.0f, rz - 1.0f});
    // clang-format on

    const float u = m_step(rx);

    const float p00 = gf::lerp(p000, p100, u);
    const float p10 = gf::lerp(p010, p110, u);
    const float p01 = gf::lerp(p001, p101, u);
    const float p11 = gf::lerp(p011, p111, u);

    const float v = m_step(ry);

    const float p0 = gf::lerp(p00, p10, v);
    const float p1 = gf::lerp(p01, p11, v);

    const float w = m_step(rz);

    return gf::lerp(p0, p1, w);
  }

  Vec3F GradientNoise3D::at(uint8_t i, uint8_t j, uint8_t k) const
  {
    uint8_t index = i;
    index = m_permutation.at(index) + j;
    index = m_permutation.at(index) + k;
    return m_gradients.at(m_permutation.at(index));
  }

  /*
   * BetterGradientNoise2D
   */

  BetterGradientNoise2D::BetterGradientNoise2D(Random* random)
  {
    // initialize permutation

    generate_permutation(random, m_permutation_x);
    generate_permutation(random, m_permutation_y);

    // generate 2D gradients

    for (Vec2F& gradient : m_gradients) {
      const float angle = random->compute_uniform_float(2.0f * constants::Pi<float>);
      gradient = gf::unit(angle);
    }
  }

  float BetterGradientNoise2D::value(float x, float y)
  {
    float dummy = 0.0f;

    auto qx = static_cast<uint8_t>(x);
    const float rx = std::modf(x, &dummy);
    assert(rx >= 0.0f && rx <= 1.0f);

    auto qy = static_cast<uint8_t>(y);
    const float ry = std::modf(y, &dummy);
    assert(ry >= 0.0f && ry <= 1.0f);

    float value = 0.0f;

    for (int j = -1; j <= 1; ++j) {
      for (int i = -1; i <= 1; ++i) {
        const Vec2F r(rx - static_cast<float>(i), ry - static_cast<float>(j));
        const float d = gf::square_length(r);

        if (d < 4.0f) {
          const float t = 1.0f - (d / 4.0f);
          value += dot(at(static_cast<uint8_t>(qx + i), static_cast<uint8_t>(qy + j)), r) * t * t * t * t * ((4.0f * t) - 3.0f);
        }
      }
    }

    return value;
  }

  Vec2F BetterGradientNoise2D::at(uint8_t i, uint8_t j) const
  {
    const uint8_t index = m_permutation_x.at(i) ^ m_permutation_y.at(j);
    return m_gradients.at(index);
  }

  /*
   * FractalNoise2D
   */

  FractalNoise2D::FractalNoise2D(Noise2D* noise, float scale, int octaves, float lacunarity, float persistence, float dimension)
  : m_noise(noise)
  , m_scale(scale)
  , m_octaves(octaves)
  , m_lacunarity(lacunarity)
  , m_persistence(persistence)
  , m_dimension(dimension)
  {
  }

  float FractalNoise2D::value(float x, float y)
  {
    if (m_noise == nullptr) {
      return 0.0f;
    }

    float value = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f;

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

  FractalNoise3D::FractalNoise3D(Noise3D* noise, float scale, int octaves, float lacunarity, float persistence, float dimension)
  : m_noise(noise)
  , m_scale(scale)
  , m_octaves(octaves)
  , m_lacunarity(lacunarity)
  , m_persistence(persistence)
  , m_dimension(dimension)
  {
  }

  float FractalNoise3D::value(float x, float y, float z)
  {
    if (m_noise == nullptr) {
      return 0.0f;
    }

    float value = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f;

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

  PerlinNoise2D::PerlinNoise2D(Random* random, float scale, int octaves)
  : m_gradient_noise(random, gf::quintic_step)
  , m_fractal_noise(&m_gradient_noise, scale, octaves)
  {
  }

  float PerlinNoise2D::value(float x, float y)
  {
    return m_fractal_noise(x, y);
  }

  /*
   * PerlinNoise3D
   */

  PerlinNoise3D::PerlinNoise3D(Random* random, float scale, int octaves)
  : m_gradient_noise(random, gf::quintic_step)
  , m_fractal_noise(&m_gradient_noise, scale, octaves)
  {
  }

  float PerlinNoise3D::value(float x, float y, float z)
  {
    return m_fractal_noise(x, y, z);
  }

  /*
   * SimplexNoise2D
   */

  SimplexNoise2D::SimplexNoise2D(Random* random)
  {
    generate_permutation(random, m_permutation);
  }

  float SimplexNoise2D::value(float x, float y)
  {
    static constexpr float F2 = .366025403784438646763723170752f; // (sqrt(3) - 1) / 2
    static constexpr float G2 = .211324865405187117745425609748f; // K / (1 + 2 * K)

    const float s = (x + y) * F2;
    float i = x + s;
    float j = y + s;

    i = std::floor(i);
    j = std::floor(j);

    const float t = (i + j) * G2;
    float x0 = i - t;
    float y0 = j - t;

    x0 = x - x0;
    y0 = y - y0;

    uint8_t i1 = 0;
    uint8_t j1 = 0;

    if (x0 > y0) {
      i1 = 1;
    } else {
      j1 = 1;
    }

    const float x1 = x0 - static_cast<float>(i1) + G2;
    const float y1 = y0 - static_cast<float>(j1) + G2;

    const float x2 = x0 - 1 + (2 * G2);
    const float y2 = y0 - 1 + (2 * G2);

    auto ii = static_cast<uint8_t>(i);
    auto jj = static_cast<uint8_t>(j);

    float res = 0.0f;

    float d0 = 0.5f - (x0 * x0) - (y0 * y0);

    if (d0 > 0) {
      d0 *= d0;
      res += d0 * d0 * dot(at(ii, jj), { x0, y0 });
    }

    float d1 = 0.5f - (x1 * x1) - (y1 * y1);

    if (d1 > 0) {
      d1 *= d1;
      res += d1 * d1 * dot(at(ii + i1, jj + j1), { x1, y1 });
    }

    float d2 = 0.5f - (x2 * x2) - (y2 * y2);

    if (d2 > 0) {
      d2 *= d2;
      res += d2 * d2 * dot(at(ii + 1, jj + 1), { x2, y2 });
    }

    return 45.23065f * res;
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

  Vec2F SimplexNoise2D::at(uint8_t i, uint8_t j) const
  {
    // clang-format off
    static constexpr std::array<Vec2F, 8> Gradients = {
      gf::vec( 1.0f,  2.0f),
      gf::vec(-1.0f,  2.0f),
      gf::vec( 1.0f, -2.0f),
      gf::vec(-1.0f, -2.0f),
      gf::vec( 2.0f,  1.0f),
      gf::vec( 2.0f, -1.0f),
      gf::vec(-2.0f,  1.0f),
      gf::vec(-2.0f, -1.0f)
    };
    // clang-format on

    uint8_t index = i;
    index = m_permutation.at(index) + j;
    return Gradients.at(m_permutation.at(index) % 8);
  }

  /*
   * WaveletNoise3D
   */

  namespace {

    std::ptrdiff_t positive_mod(std::ptrdiff_t x, std::ptrdiff_t n)
    {
      const std::ptrdiff_t r = x % n;
      return r < 0 ? r + n : r;
    }

    void wavelet_downsample(const float* from, float* to, std::ptrdiff_t n, std::ptrdiff_t stride)
    {
      static constexpr std::ptrdiff_t DownCoefficientsCount = 16;

      static constexpr std::array<float, 2 * DownCoefficientsCount> DownCoefficients = {
        // clang-format off
        0.000334f, -0.001528f,  0.000410f,  0.003545f, -0.000938f, -0.008233f,  0.002172f,  0.019120f,
       -0.005040f, -0.044412f,  0.011655f,  0.103311f, -0.025936f, -0.243780f,  0.033979f,  0.655340f,
        0.655340f,  0.033979f, -0.243780f, -0.025936f,  0.103311f,  0.011655f, -0.044412f, -0.005040f,
        0.019120f,  0.002172f, -0.008233f, -0.000938f,  0.003546f,  0.000410f, -0.001528f,  0.000334f,
        // clang-format on
      };

      const float* coeffs = &DownCoefficients[DownCoefficientsCount];

      for (std::ptrdiff_t i = 0; i < n / 2; ++i) {
        float value = 0;

        for (std::ptrdiff_t k = (2 * i) - DownCoefficientsCount; k <= (2 * i) - DownCoefficientsCount; ++k) {
          const std::ptrdiff_t index = k - (2 * i);
          assert(-DownCoefficientsCount <= index && index < DownCoefficientsCount);
          value += coeffs[index] * from[positive_mod(k, n) * stride];
        }

        to[i * stride] = value;
      }
    }

    void wavelet_upsample(const float* from, float* to, std::ptrdiff_t n, std::ptrdiff_t stride)
    {
      static constexpr std::ptrdiff_t UpCoefficientsCount = 2;

      static constexpr std::array<float, 2 * UpCoefficientsCount> UpCoefficients = {
        0.25f, 0.75f, 0.75f, 0.25f
      };

      const float* coeffs = &UpCoefficients[UpCoefficientsCount];

      for (std::ptrdiff_t i = 0; i < n; ++i) {
        float value = 0;

        for (std::ptrdiff_t k = i / 2; k <= (i / 2) + 1; ++k) {
          const std::ptrdiff_t index = i - (2 * k);
          assert(-UpCoefficientsCount <= index && index < UpCoefficientsCount);
          value += coeffs[index] * from[positive_mod(k, n / 2) * stride];
        }

        to[i * stride] = value;
      }
    }

  }

  WaveletNoise3D::WaveletNoise3D(Random* random, std::ptrdiff_t wavelet_tile_size)
  : m_wavelet_tile_size(wavelet_tile_size + (wavelet_tile_size % 2))
  {
    const std::size_t data_size = m_wavelet_tile_size * m_wavelet_tile_size * m_wavelet_tile_size;
    m_data.resize(data_size);

    std::vector<float> tmp1(data_size);
    std::vector<float> tmp2(data_size);

    // step 1: fill the tile with numbers in the range -1 to 1

    for (auto& value : m_data) {
      value = random->compute_uniform_float(-1.0f, +1.0f);
    }

    // step 2 and 3: downsample and upsample the tile

    for (std::ptrdiff_t iy = 0; iy < m_wavelet_tile_size; ++iy) {
      for (std::ptrdiff_t iz = 0; iz < m_wavelet_tile_size; ++iz) {
        // each x row
        const std::ptrdiff_t i = (iy * m_wavelet_tile_size) + (iz * m_wavelet_tile_size * m_wavelet_tile_size);
        wavelet_downsample(&m_data[i], &tmp1[i], m_wavelet_tile_size, 1);
        wavelet_upsample(&tmp1[i], &tmp2[i], m_wavelet_tile_size, 1);
      }
    }

    for (std::ptrdiff_t ix = 0; ix < m_wavelet_tile_size; ++ix) {
      for (std::ptrdiff_t iz = 0; iz < m_wavelet_tile_size; ++iz) {
        // each y row
        const std::ptrdiff_t i = ix + (iz * m_wavelet_tile_size * m_wavelet_tile_size);
        wavelet_downsample(&tmp2[i], &tmp1[i], m_wavelet_tile_size, m_wavelet_tile_size);
        wavelet_upsample(&tmp1[i], &tmp2[i], m_wavelet_tile_size, m_wavelet_tile_size);
      }
    }

    for (std::ptrdiff_t ix = 0; ix < m_wavelet_tile_size; ++ix) {
      for (std::ptrdiff_t iy = 0; iy < m_wavelet_tile_size; ++iy) {
        // each z row
        const std::ptrdiff_t i = ix + (iy * m_wavelet_tile_size);
        wavelet_downsample(&tmp2[i], &tmp1[i], m_wavelet_tile_size, m_wavelet_tile_size * m_wavelet_tile_size);
        wavelet_upsample(&tmp1[i], &tmp2[i], m_wavelet_tile_size, m_wavelet_tile_size * m_wavelet_tile_size);
      }
    }

    // step 4: substract out the coarse-scale contribution

    for (std::ptrdiff_t i = 0; i < m_wavelet_tile_size * m_wavelet_tile_size * m_wavelet_tile_size; ++i) {
      m_data[i] -= tmp2[i];
    }

    // avoid event/odd variance difference by adding off-offset version of noise to itself

    std::ptrdiff_t offset = m_wavelet_tile_size / 2;

    if (offset % 2 == 0) {
      ++offset;
    }

    std::ptrdiff_t k = 0;

    for (std::ptrdiff_t ix = 0; ix < m_wavelet_tile_size; ++ix) {
      for (std::ptrdiff_t iy = 0; iy < m_wavelet_tile_size; ++iy) {
        for (std::ptrdiff_t iz = 0; iz < m_wavelet_tile_size; ++iz) {
          // clang-format off
          const std::ptrdiff_t index = positive_mod(ix + offset, m_wavelet_tile_size)
                                     + (positive_mod(iy + offset, m_wavelet_tile_size) * m_wavelet_tile_size)
                                     + (positive_mod(iz + offset, m_wavelet_tile_size) * m_wavelet_tile_size * m_wavelet_tile_size);
          // clang-format on
          assert(0 <= index && index < m_wavelet_tile_size * m_wavelet_tile_size * m_wavelet_tile_size);
          tmp1[k++] = m_data[index];
        }
      }
    }

    for (std::ptrdiff_t i = 0; i < m_wavelet_tile_size * m_wavelet_tile_size * m_wavelet_tile_size; ++i) {
      m_data[i] += tmp1[i];
    }
  }

  float WaveletNoise3D::value(float x, float y, float z)
  {
    const float p[3] = { x, y, z };

    std::ptrdiff_t f[3];
    std::ptrdiff_t c[3];
    std::ptrdiff_t mid[3];

    float w[3][3];
    float value = 0;

    // evaluate quadratic B-spline basis functions

    for (std::ptrdiff_t i = 0; i < 3; ++i) {
      mid[i] = static_cast<std::ptrdiff_t>(std::ceil(p[i] - 0.5f));
      const float t = static_cast<float>(mid[i]) - (p[i] - 0.5f);
      w[i][0] = t * t / 2;
      w[i][2] = (1 - t) * (1 - t) / 2;
      w[i][1] = 1 - w[i][0] - w[i][2];
    }

    // evaluate noise by weighting noise coefficients by basis function values

    for (f[2] = -1; f[2] <= 1; ++f[2]) {
      for (f[1] = -1; f[1] <= 1; ++f[1]) {
        for (f[0] = -1; f[0] <= 1; ++f[0]) {
          float weight = 1;

          for (std::ptrdiff_t i = 0; i < 3; ++i) {
            c[i] = positive_mod(mid[i] + f[i], m_wavelet_tile_size);
            weight *= w[i][f[i] + 1];
          }

          value += weight * m_data[c[0] + (c[1] * m_wavelet_tile_size) + (c[2] * m_wavelet_tile_size * m_wavelet_tile_size)];
        }
      }
    }

    return value;
  }

  /*
   * WorleyNoise2D
   */

  WorleyNoise2D::WorleyNoise2D(Random* random, std::size_t points_count, Distance2<float> distance, std::vector<float> coefficients)
  : m_points_count(points_count)
  , m_distance(distance)
  , m_coefficients(std::move(coefficients))
  {
    // generate cells

    m_cells.reserve(m_points_count * 4);

    for (std::size_t i = 0; i < m_points_count; ++i) {
      auto x = random->compute_uniform_float<float>();
      auto y = random->compute_uniform_float<float>();

      m_cells.emplace_back(x, y);

      // clang-format off
      if (x < 0.5) {
        if (y < 0.5) {
          m_cells.emplace_back(x + 1.0f, y       );
          m_cells.emplace_back(x       , y + 1.0f);
          m_cells.emplace_back(x + 1.0f, y + 1.0f);
        } else {
          m_cells.emplace_back(x + 1.0f, y       );
          m_cells.emplace_back(x       , y - 1.0f);
          m_cells.emplace_back(x + 1.0f, y - 1.0f);
        }
      } else {
        if (y < 0.5) {
          m_cells.emplace_back(x - 1.0f, y       );
          m_cells.emplace_back(x       , y + 1.0f);
          m_cells.emplace_back(x - 1.0f, y + 1.0f);
        } else {
          m_cells.emplace_back(x - 1.0f, y       );
          m_cells.emplace_back(x       , y - 1.0f);
          m_cells.emplace_back(x - 1.0f, y - 1.0f);
        }
      }
      // clang-format on
    }

    // some sanity checks

    if (m_coefficients.empty()) {
      m_coefficients.push_back(1.0);
    }

    if (m_coefficients.size() > m_cells.size()) {
      m_coefficients.resize(m_cells.size());
    }
  }

  float WorleyNoise2D::value(float x, float y)
  {
    const float rx = std::fmod(x, 1.0f);
    const float ry = std::fmod(y, 1.0f);

    auto size = m_coefficients.size();

    const Vec2F here(rx, ry);

    std::partial_sort(m_cells.begin(), std::next(m_cells.begin(), static_cast<std::ptrdiff_t>(size)), m_cells.end(), [here, this](const Vec2F& lhs, const Vec2F& rhs) {
      return m_distance(here, lhs) < m_distance(here, rhs);
    });

    float value = 0.0;

    for (std::size_t i = 0; i < size; ++i) {
      value += m_coefficients[i] * m_distance(here, m_cells[i]);
    }

    return value;
  }

  /*
   * Multifractal2D
   */

  Multifractal2D::Multifractal2D(Noise2D* noise, float scale, int octaves, float lacunarity, float persistence, float dimension)
  : m_noise(noise)
  , m_scale(scale)
  , m_octaves(octaves)
  , m_lacunarity(lacunarity)
  , m_persistence(persistence)
  , m_dimension(dimension)
  {
  }

  float Multifractal2D::value(float x, float y)
  {
    float value = 1.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f;

    x *= m_scale;
    y *= m_scale;

    /*
     * There seems to be an error in the original source code from
     * Musgrave. Blender source code provides an alternative implementation
     * that does not use the offset parameter. gf provides the Blender version.
     *
     * original: https://engineering.purdue.edu/~ebertd/texture/1stEdition/musgrave/musgrave.c
     * blender: https://developer.blender.org/diffusion/B/browse/master/source/blender/blenlib/intern/noise.c
     */

    for (int k = 0; k < m_octaves; ++k) {
      value *= (m_noise->value(x * frequency, y * frequency) * std::pow(amplitude, m_dimension)) + 1.0f;

      frequency *= m_lacunarity;
      amplitude *= m_persistence;
    }

    return value;
  }

  /*
   * HeteroTerrain2D
   */

  HeteroTerrain2D::HeteroTerrain2D(Noise2D* noise, float scale, float offset, int octaves, float lacunarity, float persistence, float dimension)
  : m_noise(noise)
  , m_scale(scale)
  , m_offset(offset)
  , m_octaves(octaves)
  , m_lacunarity(lacunarity)
  , m_persistence(persistence)
  , m_dimension(dimension)
  {
  }

  float HeteroTerrain2D::value(float x, float y)
  {
    float frequency = 1.0f;
    float amplitude = 1.0f;

    x *= m_scale;
    y *= m_scale;

    float value = m_offset + m_noise->value(x, y);

    frequency *= m_lacunarity;
    amplitude *= m_persistence;

    for (int k = 1; k < m_octaves; ++k) {
      float increment = m_noise->value(x * frequency, y * frequency) + m_offset;
      increment *= std::pow(amplitude, m_dimension);
      increment *= value;
      value += increment;

      frequency *= m_lacunarity;
      amplitude *= m_persistence;
    }

    return value;
  }

  /*
   * HybridMultifractal2D
   */

  HybridMultifractal2D::HybridMultifractal2D(Noise2D* noise, float scale, float offset, int octaves, float lacunarity, float persistence, float dimension)
  : m_noise(noise)
  , m_scale(scale)
  , m_offset(offset)
  , m_octaves(octaves)
  , m_lacunarity(lacunarity)
  , m_persistence(persistence)
  , m_dimension(dimension)
  {
  }

  float HybridMultifractal2D::value(float x, float y)
  {
    float frequency = 1.0f;
    float amplitude = 1.0f;

    x *= m_scale;
    y *= m_scale;

    float value = m_noise->value(x, y) + m_offset;
    float weight = value;

    frequency *= m_lacunarity;
    amplitude *= m_persistence;

    for (int k = 1; k < m_octaves; ++k) {
      weight = std::min(weight, 1.0f);

      const float signal = (m_noise->value(x * frequency, y * frequency) + m_offset) * std::pow(amplitude, m_dimension);
      value += weight * signal;
      weight *= signal;

      frequency *= m_lacunarity;
      amplitude *= m_persistence;
    }

    return value;
  }

  /*
   * RidgedMultifractal2D
   */

  RidgedMultifractal2D::RidgedMultifractal2D(Noise2D* noise, float scale, float offset, float gain, int octaves, float lacunarity, float persistence, float dimension)
  : m_noise(noise)
  , m_scale(scale)
  , m_offset(offset)
  , m_gain(gain)
  , m_octaves(octaves)
  , m_lacunarity(lacunarity)
  , m_persistence(persistence)
  , m_dimension(dimension)
  {
  }

  float RidgedMultifractal2D::value(float x, float y)
  {
    float frequency = 1.0f;
    float amplitude = 1.0f;

    x *= m_scale;
    y *= m_scale;

    float signal = m_noise->value(x, y);
    signal = std::abs(signal);
    signal = m_offset - signal;
    signal *= signal;

    float value = signal;

    frequency *= m_lacunarity;
    amplitude *= m_persistence;

    for (int k = 1; k < m_octaves; ++k) {
      float weight = signal * m_gain;
      weight = gf::clamp(weight, 0.0f, 1.0f);

      signal = m_noise->value(x * frequency, y * frequency);
      signal = std::abs(signal);
      signal = m_offset - signal;
      signal *= signal;

      signal *= weight;
      value += signal * std::pow(amplitude, m_dimension);

      frequency *= m_lacunarity;
      amplitude *= m_persistence;
    }

    return value;
  }

  /*
   * Noise3DTo2DAdapter
   */

  Noise3DTo2DAdapter::Noise3DTo2DAdapter(Noise3D* noise, Vec3F normal, Vec3F point)
  : m_noise(noise)
  , m_normal(normal)
  , m_point(point)
  {
  }

  float Noise3DTo2DAdapter::value(float x, float y)
  {
    float z = 0.0f;

    if (std::abs(m_normal.z) > gf::Epsilon) {
      z = m_point.z + (((m_normal.x * (m_point.x - x)) + (m_normal.y * (m_point.y - y))) / m_normal.z);
    }

    return m_noise->value(x, y, z);
  }

}
