// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <gf2/Transform.h>

#include <cmath>

namespace gf {

  Mat3F Transform::compute_matrix(RectF bounds) const {
    const float px = position.x;
    const float py = position.y;
    const float ox = origin.x * bounds.extent.w + bounds.offset.x;
    const float oy = origin.y * bounds.extent.h + bounds.offset.y;
    const float cos_v = std::cos(rotation);
    const float sin_v = std::sin(rotation);
    const float sx = scale.x;
    const float sy = scale.y;

    Mat3F mat;
    mat(0, 0) = sx * cos_v; mat(0, 1) = - sy * sin_v; mat(0, 2) = - ox * mat(0, 0) - oy * mat(0, 1) + px;
    mat(1, 0) = sx * sin_v; mat(1, 1) =   sy * cos_v; mat(1, 2) = - ox * mat(1, 0) - oy * mat(1, 1) + py;
    mat(2, 0) = 0.0f;       mat(2, 1) = 0.0f;         mat(2, 2) = 1.0f;
    return mat;
  }

}
