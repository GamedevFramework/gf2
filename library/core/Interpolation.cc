// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/core/Interpolation.h>
// clang-format on

#include <cassert>
#include <cmath>

namespace gf {

  Vec2F quadratic_bezier_interpolation(Vec2F p0, Vec2F p1, Vec2F p2, float t)
  {
    assert(0.0f <= t && t <= 1.0f);
    return p0 * (1 - t) * (1 - t) + p1 * 2.0f * (1 - t) * t + p2 * t * t;
  }

  Vec2F cubic_bezier_interpolation(Vec2F p0, Vec2F p1, Vec2F p2, Vec2F p3, float t)
  {
    assert(0.0f <= t && t <= 1.0f);
    // clang-format off
    return p0 * (1 - t) * (1 - t) * (1 - t)
         + p1 * (1 - t) * (1 - t) * t * 3.0f
         + p2 * (1 - t) * t * t * 3.0f
         + p3 * t * t * t;
    // clang-format on
  }

  float cattmull_rom_time_difference(Vec2F p0, Vec2F p1, CattmullRomType type)
  {
    switch (type) {
      case CattmullRomType::Uniform:
        return 1.0f;
      case CattmullRomType::Chordal:
        return euclidean_distance(p0, p1);
      case CattmullRomType::Centripetal:
        return std::sqrt(euclidean_distance(p0, p1));
    }

    assert(false);
    return 1.0f;
  }

  Vec2F cattmull_rom_interpolation(Vec2F p0, float t0, Vec2F p1, float t1, Vec2F p2, float t2, Vec2F p3, float t3, float t)
  {
    // see https://en.wikipedia.org/wiki/Centripetal_Catmull%E2%80%93Rom_spline
    const Vec2F a1 = (t1 - t) / (t1 - t0) * p0 + (t - t0) / (t1 - t0) * p1;
    const Vec2F a2 = (t2 - t) / (t2 - t1) * p1 + (t - t1) / (t2 - t1) * p2;
    const Vec2F a3 = (t3 - t) / (t3 - t2) * p2 + (t - t2) / (t3 - t2) * p3;
    const Vec2F b1 = (t2 - t) / (t2 - t0) * a1 + (t - t0) / (t2 - t0) * a2;
    const Vec2F b2 = (t3 - t) / (t3 - t1) * a2 + (t - t1) / (t3 - t1) * a3;
    return (t2 - t) / (t2 - t1) * b1 + (t - t1) / (t2 - t1) * b2;
  }

}
