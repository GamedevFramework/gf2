// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_MAT4_H
#define GF_MAT4_H

#include <cassert>
#include <cstdint>

namespace gf {

  template<typename T>
  struct Mat4 {
    T m[4][4]; // row-major

    const T& operator()(int32_t row, int32_t col) const
    {
      assert(0 <= row && row < 4);
      assert(0 <= col && col < 4);
      return m[row][col];
    }

    T& operator()(int32_t row, int32_t col)
    {
      assert(0 <= row && row < 4);
      assert(0 <= col && col < 4);
      return m[row][col];
    }
  };

  template<typename T>
  inline Mat4<T> operator*(const Mat4<T>& lhs, const Mat4<T>& rhs)
  {
    Mat4<T> result = {};

    for (int32_t col = 0; col < 4; ++col) {
      for (int32_t row = 0; row < 4; ++row) {
        T sum = T(0);

        for (int32_t k = 0; k < 4; ++k) {
          sum += lhs(row, k) * rhs(k, col);
        }

        result(row, col) = sum;
      }
    }

    return result;
  }

  using Mat4F = Mat4<float>;

  inline constexpr Mat4F Identity4F = {
    { { 1.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } }
  };

} // namespace gf

#endif // GF_MAT4_H
