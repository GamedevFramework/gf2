// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Vec2.h>
// clang-format on

namespace gf {

#ifndef _MSC_VER
  template struct Vec2<float>;
  template struct Vec2<double>;
  template struct Vec2<int>;
  template struct Vec2<unsigned>;
#endif

} // namespace gf
