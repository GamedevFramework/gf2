// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/Transform.h>

#include <cmath>

namespace gf {

  Mat3F Transform::compute_matrix(RectF bounds) const
  {
    const float lx = location.x;
    const float ly = location.y;
    const float ox = origin.x * bounds.extent.w + bounds.offset.x;
    const float oy = origin.y * bounds.extent.h + bounds.offset.y;
    const float cos_v = std::cos(rotation);
    const float sin_v = std::sin(rotation);
    const float sx = scale.x;
    const float sy = scale.y;

    Mat3F mat;
    // clang-format off
    mat(0, 0) = sx * cos_v; mat(0, 1) = - sy * sin_v; mat(0, 2) = - ox * mat(0, 0) - oy * mat(0, 1) + lx;
    mat(1, 0) = sx * sin_v; mat(1, 1) =   sy * cos_v; mat(1, 2) = - ox * mat(1, 0) - oy * mat(1, 1) + ly;
    mat(2, 0) = 0.0f;       mat(2, 1) = 0.0f;         mat(2, 2) = 1.0f;
    // clang-format on
    return mat;
  }

  Mat3F Transform::compute_inverse_matrix(RectF bounds) const
  {
    return inverse(compute_matrix(bounds));
  }

  bool transformed_contains(RectF object_bounds, const Transform& object_transform, Vec2F location)
  {
    const Mat3F matrix = object_transform.compute_inverse_matrix(object_bounds);
    const Vec2F local = transform_point(matrix, location);
    return object_bounds.contains(local);
  }

} // namespace gf
