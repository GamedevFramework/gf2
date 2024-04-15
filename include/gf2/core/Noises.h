// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
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

    double value(double x, double y) override;

  private:
    double at(uint8_t i, uint8_t j) const;

    Step<double> m_step;
    std::array<uint8_t, 256> m_permutation = {};
    std::array<double, 256> m_values = {};
  };

  class GF_CORE_API GradientNoise2D : public Noise2D {
  public:
    GradientNoise2D(Random& random, Step<double> step);

    double value(double x, double y) override;

  private:
    Vec2D at(uint8_t i, uint8_t j) const;

    Step<double> m_step;
    std::array<uint8_t, 256> m_permutation = {};
    std::array<Vec2D, 256> m_gradients = {};
  };

  class GF_CORE_API GradientNoise3D : public Noise3D {
  public:
    GradientNoise3D(Random& random, Step<double> step);

    double value(double x, double y, double z) override;

  private:
    Vec3D at(uint8_t i, uint8_t j, uint8_t k) const;

    Step<double> m_step;
    std::array<uint8_t, 256> m_permutation = {};
    std::array<Vec3D, 256> m_gradients = {};
  };

  class GF_CORE_API BetterGradientNoise2D : public Noise2D {
  public:
    BetterGradientNoise2D(Random& random);

    double value(double x, double y) override;

  private:
    Vec2D at(uint8_t i, uint8_t j) const;

    std::array<uint8_t, 256> m_permutation_x = {};
    std::array<uint8_t, 256> m_permutation_y = {};
    std::array<Vec2D, 256> m_gradients = {};
  };

  class GF_CORE_API FractalNoise2D : public Noise2D {
  public:
    FractalNoise2D(Noise2D* noise, double scale, int octaves = 8, double lacunarity = 2.0, double persistence = 0.5, double dimension = 1.0);

    double value(double x, double y) override;

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

    double value(double x, double y, double z) override;

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

    double value(double x, double y) override;

  private:
    GradientNoise2D m_gradient_noise;
    FractalNoise2D m_fractal_noise;
  };

  class GF_CORE_API PerlinNoise3D : public Noise3D {
  public:
    PerlinNoise3D(Random& random, double scale, int octaves = 8);

    double value(double x, double y, double z) override;

  private:
    GradientNoise3D m_gradient_noise;
    FractalNoise3D m_fractal_noise;
  };

  class GF_CORE_API SimplexNoise2D : public Noise2D {
  public:
    SimplexNoise2D(Random& random);

    double value(double x, double y) override;

  private:
    Vec2D at(uint8_t i, uint8_t j) const;

    std::array<uint8_t, 256> m_permutation{};
  };

}

#endif // GF_NOISES_H
