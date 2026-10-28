// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
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

    virtual float value(float x, float y) = 0;

    float operator()(float x, float y)
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

    virtual float value(float x, float y, float z) = 0;

    float operator()(float x, float y, float z)
    {
      return value(x, y, z);
    }
  };

}

#endif // GF_NOISE_H
