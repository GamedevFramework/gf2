// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GEOMETRY_H
#define GF_GEOMETRY_H

#include <array>
#include <optional>
#include <vector>

#include "CoreApi.h"
#include "Polyline.h"
#include "Span.h"
#include "Vec2.h"

namespace gf {

  template<typename T>
  using Segment = std::array<Vec2<T>, 2>;

  using SegmentI = Segment<int32_t>;
  using SegmentF = Segment<float>;

  GF_CORE_API std::vector<Polyline> compute_lines(Span<const SegmentI> segments);

  class GF_CORE_API BresenhamAlgorithm {
  public:
    BresenhamAlgorithm(Vec2I p0, Vec2I p1);

    std::optional<Vec2I> step();

  private:
    Vec2I m_p0 = { 0, 0 };
    Vec2I m_p1 = { 0, 0 };
    Vec2I m_delta = { 0, 0 };
    Vec2I m_step = { 0, 0 };
    int32_t m_error = 0;
  };

  GF_CORE_API std::vector<Vec2I> generate_line(Vec2I p0, Vec2I p1);

  class GF_CORE_API AndresAlgorithm {
  public:
    AndresAlgorithm(int32_t radius);

    std::optional<Vec2I> step();

  private:
    int32_t m_radius = 0;
    int32_t m_d = 0;
    int32_t m_x = 0;
    int32_t m_y = 0;
  };

  GF_CORE_API std::vector<Vec2I> generate_circle(Vec2I center, int32_t radius);
}

#endif // GF_GEOMETRY_H
