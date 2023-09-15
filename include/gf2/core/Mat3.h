// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_MAT3_H
#define GF_MAT3_H

#include <cstdint>

#include "CoreApi.h"
#include "Vec2.h"

namespace gf {

  template<typename T>
  struct Mat3 {
    T m[3][3]; // row-major

    const T& operator()(int32_t row, int32_t col) const
    {
      assert(0 <= row && row < 3);
      assert(0 <= col && col < 3);
      return m[row][col]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    T& operator()(int32_t row, int32_t col)
    {
      assert(0 <= row && row < 3);
      assert(0 <= col && col < 3);
      return m[row][col]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }
  };

  template<typename T>
  inline Mat3<T> operator*(const Mat3<T>& lhs, const Mat3<T>& rhs)
  {
    Mat3<T> result = {};

    for (int32_t col = 0; col < 3; ++col) {
      for (int32_t row = 0; row < 3; ++row) {
        T sum = T(0);

        for (int32_t k = 0; k < 3; ++k) {
          sum += lhs(row, k) * rhs(k, col);
        }

        result(row, col) = sum;
      }
    }

    return result;
  }

  using Mat3F = Mat3<float>;

  inline constexpr Mat3F Identity3F = {
    { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
  };

  GF_CORE_API Vec2F transform_point(const Mat3F& mat, Vec2F vec);
  GF_CORE_API Vec2F transform_vector(const Mat3F& mat, Vec2F vec);
  GF_CORE_API Mat3F inverse(const Mat3F& mat);

} // namespace gf

#endif // GF_MAT3_H
