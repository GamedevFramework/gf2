// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/Vec2.h>

namespace gf {

#ifndef _MSC_VER
  template struct Vec2<float>;
  template struct Vec2<double>;
  template struct Vec2<int32_t>;
  template struct Vec2<uint32_t>;
#endif

} // namespace gf
