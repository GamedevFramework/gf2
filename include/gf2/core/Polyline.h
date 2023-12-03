// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_POLYLINE_H
#define GF_POLYLINE_H

#include <cstdint>

#include <vector>

#include "CoreApi.h"
#include "Vec2.h"

namespace gf {

  enum class PolylineType : uint8_t {
    Chain,
    Loop,
  };

  struct GF_CORE_API Polyline {
    PolylineType type = PolylineType::Chain;
    std::vector<Vec2F> points;
  };

}

#endif // GF_POLYLINE_H
