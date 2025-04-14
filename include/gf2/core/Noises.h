// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_NOISES_H
#define GF_NOISES_H

#include <cstdint>

#include <array>

#include "CoreApi.h"
#include "Math.h"
#include "Noise.h"
#include "Random.h"
#include "Vec2.h"
#include "Vec3.h"

namespace gf {

  class GF_CORE_API ValueNoise2D : public Noise2D {
  public:
    ValueNoise2D(Random& random, Step<double> step);

    double value(double x, double y) final;

  private:
    double at(uint8_t i, uint8_t j) const;

    Step<double> m_step;
    std::array<uint8_t, 256> m_permutation = {};
    std::array<double, 256> m_values = {};
  };

  class GF_CORE_API GradientNoise2D : public Noise2D {
  public:
    GradientNoise2D(Random& random, Step<double> step);

    double value(double x, double y) final;

  private:
    Vec2D at(uint8_t i, uint8_t j) const;

    Step<double> m_step;
    std::array<uint8_t, 256> m_permutation = {};
    std::array<Vec2D, 256> m_gradients = {};
  };

  class GF_CORE_API GradientNoise3D : public Noise3D {
  public:
    GradientNoise3D(Random& random, Step<double> step);

    double value(double x, double y, double z) final;

  private:
    Vec3D at(uint8_t i, uint8_t j, uint8_t k) const;

    Step<double> m_step;
    std::array<uint8_t, 256> m_permutation = {};
    std::array<Vec3D, 256> m_gradients = {};
  };

  class GF_CORE_API BetterGradientNoise2D : public Noise2D {
  public:
    BetterGradientNoise2D(Random& random);

    double value(double x, double y) final;

  private:
    Vec2D at(uint8_t i, uint8_t j) const;

    std::array<uint8_t, 256> m_permutation_x = {};
    std::array<uint8_t, 256> m_permutation_y = {};
    std::array<Vec2D, 256> m_gradients = {};
  };

  class GF_CORE_API FractalNoise2D : public Noise2D {
  public:
    FractalNoise2D(Noise2D* noise, double scale, int octaves = 8, double lacunarity = 2.0, double persistence = 0.5, double dimension = 1.0);

    double value(double x, double y) final;

  private:
    Noise2D* m_noise = nullptr;
    double m_scale = 1.0;
    int m_octaves = 8;
    double m_lacunarity = 2.0;
    double m_persistence = 0.5;
    double m_dimension = 1.0;
  };

  class GF_CORE_API FractalNoise3D : public Noise3D {
  public:
    FractalNoise3D(Noise3D* noise, double scale, int octaves = 8, double lacunarity = 2.0, double persistence = 0.5, double dimension = 1.0);

    double value(double x, double y, double z) final;

  private:
    Noise3D* m_noise = nullptr;
    double m_scale = 1.0;
    int m_octaves = 8;
    double m_lacunarity = 2.0;
    double m_persistence = 0.5;
    double m_dimension = 1.0;
  };

  class GF_CORE_API PerlinNoise2D : public Noise2D {
  public:
    PerlinNoise2D(Random& random, double scale, int octaves = 8);

    double value(double x, double y) final;

  private:
    GradientNoise2D m_gradient_noise;
    FractalNoise2D m_fractal_noise;
  };

  class GF_CORE_API PerlinNoise3D : public Noise3D {
  public:
    PerlinNoise3D(Random& random, double scale, int octaves = 8);

    double value(double x, double y, double z) final;

  private:
    GradientNoise3D m_gradient_noise;
    FractalNoise3D m_fractal_noise;
  };

  class GF_CORE_API SimplexNoise2D : public Noise2D {
  public:
    SimplexNoise2D(Random& random);

    double value(double x, double y) final;

  private:
    Vec2D at(uint8_t i, uint8_t j) const;

    std::array<uint8_t, 256> m_permutation{};
  };

  class GF_CORE_API WaveletNoise3D : public Noise3D {
  public:
    WaveletNoise3D(Random& random, std::ptrdiff_t wavelet_tile_size = 32);

    double value(double x, double y, double z) final;

  private:
    std::ptrdiff_t m_wavelet_tile_size;
    std::vector<double> m_data;
  };

  class GF_CORE_API WorleyNoise2D : public Noise2D {
  public:
    WorleyNoise2D(Random& random, std::size_t points_count, Distance2<double> distance, std::vector<double> coefficients);

    double value(double x, double y) final;

  private:
    std::size_t m_points_count;
    Distance2<double> m_distance;
    std::vector<double> m_coefficients;
    std::vector<Vec2D> m_cells;
  };

  class GF_CORE_API Multifractal2D : public Noise2D {
  public:
    Multifractal2D(Noise2D* noise, double scale, int octaves = 8, double lacunarity = 2.0, double persistence = 0.5, double dimension = 1.0);

    double value(double x, double y) final;

  private:
    Noise2D* m_noise = nullptr;
    double m_scale = 1.0;
    int m_octaves = 8;
    double m_lacunarity = 2.0;
    double m_persistence = 0.5;
    double m_dimension = 1.0;
  };

  class GF_CORE_API HeteroTerrain2D : public Noise2D {
  public:
    HeteroTerrain2D(Noise2D* noise, double scale, double offset = 0.0, int octaves = 8, double lacunarity = 2.0, double persistence = 0.5, double dimension = 1.0);

    double value(double x, double y) final;

  private:
    Noise2D* m_noise = nullptr;
    double m_scale = 1.0;
    double m_offset = 0.0;
    int m_octaves = 8;
    double m_lacunarity = 2.0;
    double m_persistence = 0.5;
    double m_dimension = 1.0;
  };

  class GF_CORE_API HybridMultifractal2D : public Noise2D {
  public:
    HybridMultifractal2D(Noise2D* noise, double scale, double offset = 0.0, int octaves = 8, double lacunarity = 2.0, double persistence = 0.5, double dimension = 1.0);

    double value(double x, double y) final;

  private:
    Noise2D* m_noise = nullptr;
    double m_scale = 1.0;
    double m_offset = 0.0;
    int m_octaves = 8;
    double m_lacunarity = 2.0;
    double m_persistence = 0.5;
    double m_dimension = 1.0;
  };

  class GF_CORE_API RidgedMultifractal2D : public Noise2D {
  public:
    RidgedMultifractal2D(Noise2D* noise, double scale, double offset = 1.0, double gain = 1.0, int octaves = 8, double lacunarity = 2.0, double persistence = 0.5, double dimension = 1.0);

    double value(double x, double y) final;

  private:
    Noise2D* m_noise = nullptr;
    double m_scale = 1.0;
    double m_offset = 1.0;
    double m_gain = 1.0;
    int m_octaves = 8;
    double m_lacunarity = 2.0;
    double m_persistence = 0.5;
    double m_dimension = 1.0;
  };

  class GF_CORE_API Noise3DTo2DAdapter : public Noise2D {
  public:
    Noise3DTo2DAdapter(Noise3D* noise, Vec3D normal = { 0.0, 0.0, 1.0 }, Vec3D point = { 0.0, 0.0, 0.0 });

    double value(double x, double y) final;

  private:
    Noise3D* m_noise;
    Vec3D m_normal;
    Vec3D m_point;
  };

}

#endif // GF_NOISES_H
