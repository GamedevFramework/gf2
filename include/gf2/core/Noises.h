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


}

#endif // GF_NOISES_H
