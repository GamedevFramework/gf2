// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/core/Circ.h>
// clang-format on

namespace gf {

#ifndef _MSC_VER
  template struct Circ<float>;
  template struct Circ<double>;
  template struct Circ<int>;
  template struct Circ<unsigned>;
#endif

} // namespace gf
