// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_INTERPOLATION_H
#define GF_INTERPOLATION_H

#include <cstdint>

#include "CoreApi.h"
#include "Vec2.h"

namespace gf {

  GF_CORE_API Vec2F quadratic_bezier_interpolation(Vec2F p0, Vec2F p1, Vec2F p2, float t);
  GF_CORE_API Vec2F cubic_bezier_interpolation(Vec2F p0, Vec2F p1, Vec2F p2, Vec2F p3, float t);

  enum class CattmullRomType : uint8_t {
    Uniform,
    Chordal,
    Centripetal,
  };

  GF_CORE_API float cattmull_rom_time_difference(Vec2F p0, Vec2F p1, CattmullRomType type = CattmullRomType::Centripetal);

  GF_CORE_API Vec2F cattmull_rom_interpolation(Vec2F p0, float t0, Vec2F p1, float t1, Vec2F p2, float t2, Vec2F p3, float t3, float t);

}

#endif // GF_INTERPOLATION_H
