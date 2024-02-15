// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/Rect.h>

namespace gf {

#ifndef _MSC_VER
  template struct Rect<float>;
  template struct Rect<double>;
  template struct Rect<int32_t>;
  template struct Rect<uint32_t>;
#endif

} // namespace gf
