// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Circ.h>

namespace gf {

#ifndef _MSC_VER
  template struct Circ<float>;
  template struct Circ<double>;
  template struct Circ<int32_t>;
  template struct Circ<uint32_t>;
#endif

} // namespace gf
