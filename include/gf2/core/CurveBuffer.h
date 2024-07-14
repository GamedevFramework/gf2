// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CURVE_BUFFER_H
#define GF_CURVE_BUFFER_H

#include <cstdint>

#include <vector>

#include "Color.h"
#include "CoreApi.h"
#include "Interpolation.h"
#include "Polyline.h"
#include "Span.h"
#include "TypeTraits.h"
#include "Vec2.h"

namespace gf {

  struct CompoundCurveBuilder {
    CompoundCurveBuilder(Vec2F origin);

    CompoundCurveBuilder& set_origin(Vec2F origin);

    CompoundCurveBuilder& line_to(Vec2F p1);
    CompoundCurveBuilder& quadratic_bezier_curve_to(Vec2F p1, Vec2F p2, uint32_t point_count = 20);
    CompoundCurveBuilder& cubic_bezier_curve_to(Vec2F p1, Vec2F p2, Vec2F p3, uint32_t point_count = 30);

    CompoundCurveBuilder& relative_line_to(Vec2F d1);
    CompoundCurveBuilder& relative_quadratic_bezier_curve_to(Vec2F d1, Vec2F d2, uint32_t point_count = 20);
    CompoundCurveBuilder& relative_cubic_bezier_curve_to(Vec2F d1, Vec2F d2, Vec2F d3, uint32_t point_count = 30);

    void close();
    void clear(Vec2F origin);

    std::vector<Vec2F> points;
    PolylineType type = PolylineType::Chain;
  };

  struct GF_CORE_API CurveBuffer {
    static CurveBuffer make_line(Vec2F p0, Vec2F p1);
    static CurveBuffer make_compound_curve(Span<const Vec2F> points, PolylineType type = PolylineType::Chain);
    static CurveBuffer make_quadratic_bezier(Vec2F p0, Vec2F p1, Vec2F p2, uint32_t point_count = 20);
    static CurveBuffer make_cubic_bezier(Vec2F p0, Vec2F p1, Vec2F p2, Vec2F p3, uint32_t point_count = 30);
    static CurveBuffer make_cattmull_rom_spline(Span<const Vec2F> points, PolylineType type = PolylineType::Chain, CattmullRomType spline_type = CattmullRomType::Centripetal, uint32_t point_count = 30);

    std::vector<Vec2F> points;
    PolylineType type = PolylineType::Chain;
    float thickness = 1.0f;
    Color color = Black;
    float outline_thickness = 0.0f;
    Color outline_color = White;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<CurveBuffer, Archive>& buffer)
  {
    return ar | buffer.points | buffer.type | buffer.thickness | buffer.color | buffer.outline_thickness | buffer.outline_color;
  }

  struct GF_CORE_API CurveGroupBuffer {
    std::vector<CurveBuffer> curves;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<CurveGroupBuffer, Archive>& buffer)
  {
    return ar | buffer.curves;
  }

}

#endif // GF_CURVE_BUFFER_H
