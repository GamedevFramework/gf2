// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/core/Mat3.h>
// clang-format on

namespace gf {

  static_assert(sizeof(Mat3F) == 9 * sizeof(float), "Size mismatch");

  Vec2F transform_point(const Mat3F& mat, Vec2F vec)
  {
    Vec2F result;
    result.x = mat(0, 0) * vec.x + mat(0, 1) * vec.y + mat(0, 2);
    result.y = mat(1, 0) * vec.x + mat(1, 1) * vec.y + mat(1, 2);
    return result;
  }

  Vec2F transform_vector(const Mat3F& mat, Vec2F vec)
  {
    Vec2F result;
    result.x = mat(0, 0) * vec.x + mat(0, 1) * vec.y;
    result.y = mat(1, 0) * vec.x + mat(1, 1) * vec.y;
    return result;
  }

  Mat3F inverse(const Mat3F& mat)
  {
    Mat3F result;
    result(0, 0) = mat(1, 1) * mat(2, 2) - mat(2, 1) * mat(1, 2);
    result(0, 1) = -(mat(0, 1) * mat(2, 2) - mat(2, 1) * mat(0, 2));
    result(0, 2) = mat(0, 1) * mat(1, 2) - mat(1, 1) * mat(0, 2);
    result(1, 0) = -(mat(1, 0) * mat(2, 2) - mat(2, 0) * mat(1, 2));
    result(1, 1) = mat(0, 0) * mat(2, 2) - mat(2, 0) * mat(0, 2);
    result(1, 2) = -(mat(0, 0) * mat(1, 2) - mat(1, 0) * mat(0, 2));
    result(2, 0) = mat(1, 0) * mat(2, 1) - mat(2, 0) * mat(1, 1);
    result(2, 1) = -(mat(0, 0) * mat(2, 1) - mat(2, 0) * mat(0, 1));
    result(2, 2) = mat(0, 0) * mat(1, 1) - mat(1, 0) * mat(0, 1);

    const float det = mat(0, 0) * result(0, 0) + mat(0, 1) * result(1, 0) + mat(0, 2) * result(2, 0);
    assert(std::abs(det) > std::numeric_limits<float>::epsilon());

    for (int col = 0; col < 3; ++col) {
      for (int row = 0; row < 3; ++row) {
        result(row, col) /= det;
      }
    }

    return result;
  }

} // namespace gf
