// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Vec3.h>

namespace gf {

#ifndef _MSC_VER
  template struct Vec3<float>;
  template struct Vec3<double>;
  template struct Vec3<int32_t>;
  template struct Vec3<uint32_t>;
#endif

} // namespace gf
