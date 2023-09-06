// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <gf2/core/Rect.h>

namespace gf {

#ifndef _MSC_VER
  template struct Rect<float>;
  template struct Rect<double>;
  template struct Rect<int>;
  template struct Rect<unsigned>;
#endif

} // namespace gf
