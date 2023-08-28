// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_MAT4_H
#define GF_MAT4_H

#include <cassert>

#include "Mat3.h"

namespace gf {

  template<typename T>
  struct Mat4 {
    T m[4][4]; // row-major

    const T& operator()(int row, int col) const
    {
      assert(0 <= row && row < 4);
      assert(0 <= col && col < 4);
      return m[row][col]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    T& operator()(int row, int col)
    {
      assert(0 <= row && row < 4);
      assert(0 <= col && col < 4);
      return m[row][col]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    Mat4() = default;

    Mat4(const Mat3<T>& other)
    : m{
      { other(0, 0), other(0, 1), other(0, 2), 0.0f },
      { other(1, 0), other(1, 1), other(1, 2), 0.0f },
      { other(2, 0), other(2, 1), other(2, 2), 0.0f },
      { 0.0f, 0.0f, 0.0f, 0.0f }
    }
    {
    }
  };

  using Mat4F = Mat4<float>;

} // namespace gf

#endif // GF_MAT4_H
