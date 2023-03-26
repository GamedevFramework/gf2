// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <gf2/Vec3.h>

namespace gf {

#ifndef _MSC_VER
  template struct Vec3<float>;
  template struct Vec3<double>;
  template struct Vec3<int>;
  template struct Vec3<unsigned>;
#endif

}
