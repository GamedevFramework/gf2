// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <gf2/Circ.h>

namespace gf {

#ifndef _MSC_VER
  template struct Circ<float>;
  template struct Circ<double>;
  template struct Circ<int>;
  template struct Circ<unsigned>;
#endif

} // namespace gf
