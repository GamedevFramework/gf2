// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_NOISE_H
#define GF_NOISE_H

#include "CoreApi.h"

namespace gf {

  class GF_CORE_API Noise2D {
  public:
    Noise2D() = default;
    Noise2D(const Noise2D&) = default;
    Noise2D(Noise2D&&) noexcept = default;
    virtual ~Noise2D();

    Noise2D& operator=(const Noise2D&) = default;
    Noise2D& operator=(Noise2D&&) noexcept = default;

    virtual double value(double x, double y) = 0;

    double operator()(double x, double y)
    {
      return value(x, y);
    }
  };

  class GF_CORE_API Noise3D {
  public:
    Noise3D() = default;
    Noise3D(const Noise3D&) = default;
    Noise3D(Noise3D&&) noexcept = default;
    virtual ~Noise3D();

    Noise3D& operator=(const Noise3D&) = default;
    Noise3D& operator=(Noise3D&&) noexcept = default;

    virtual double value(double x, double y, double z) = 0;

    double operator()(double x, double y, double z)
    {
      return value(x, y, z);
    }
  };

}

#endif // GF_NOISE_H
