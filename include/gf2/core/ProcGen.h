// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PROC_GEN_H
#define GF_PROC_GEN_H

#include <vector>

#include "Heightmap.h"
#include "Random.h"
#include "Span.h"
#include "Vec2.h"

namespace gf {

  GF_CORE_API std::vector<Vec2F> midpoint_displacement_1d(Vec2F p0, Vec2F p1, Random& random, int iterations, Vec2F direction, float initial_factor = 1.0f, float reduction_factor = 0.5f);

  GF_CORE_API std::vector<Vec2F> midpoint_displacement_1d(Vec2F p0, Vec2F p1, Random& random, int iterations, float initial_factor = 1.0f, float reduction_factor = 0.5f);

  GF_CORE_API Heightmap midpoint_displacement_2d(Vec2I size, Random& random, Span<const double> initial_values = nullptr);

  GF_CORE_API Heightmap diamond_square_2d(Vec2I size, Random& random, Span<const double> initial_values = nullptr);

}

#endif // GF_PROC_GEN_H
