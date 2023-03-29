// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Vec3.h>
// clang-format on

namespace gf {

#ifndef _MSC_VER
  template struct Vec3<float>;
  template struct Vec3<double>;
  template struct Vec3<int>;
  template struct Vec3<unsigned>;
#endif

} // namespace gf
