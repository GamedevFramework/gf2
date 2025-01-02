// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_POLYLINE_H
#define GF_POLYLINE_H

#include <cstdint>

#include <vector>

#include "CoreApi.h"
#include "Span.h"
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

  struct GF_CORE_API PolylineView {
    PolylineType type = PolylineType::Chain;
    Span<const Vec2F> points;

    bool has_prev(std::size_t i) const;
    Vec2F prev_point(std::size_t i) const;
    bool has_next(std::size_t i) const;
    Vec2F next_point(std::size_t i) const;
  };

}

#endif // GF_POLYLINE_H
