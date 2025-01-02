// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/CurveBuffer.h>

#include <cassert>

namespace gf {

  CompoundCurveBuilder::CompoundCurveBuilder(Vec2F origin)
  {
    points.emplace_back(origin);
  }

  CompoundCurveBuilder& CompoundCurveBuilder::set_origin(Vec2F origin)
  {
    assert(!points.empty());
    points[0] = origin;
    return *this;
  }

  CompoundCurveBuilder& CompoundCurveBuilder::line_to(Vec2F p1)
  {
    assert(!points.empty());
    points.push_back(p1);
    return *this;
  }

  CompoundCurveBuilder& CompoundCurveBuilder::quadratic_bezier_curve_to(Vec2F p1, Vec2F p2, uint32_t point_count)
  {
    assert(!points.empty());
    assert(point_count >= 2);

    const Vec2F p0 = points.back();

    for (uint32_t i = 1; i < point_count; ++i) {
      const float t = static_cast<float>(i) / static_cast<float>(point_count - 1);
      points.emplace_back(quadratic_bezier_interpolation(p0, p1, p2, t));
    }

    return *this;
  }

  CompoundCurveBuilder& CompoundCurveBuilder::cubic_bezier_curve_to(Vec2F p1, Vec2F p2, Vec2F p3, uint32_t point_count)
  {
    assert(!points.empty());
    assert(point_count >= 2);

    const Vec2F p0 = points.back();

    for (uint32_t i = 1; i < point_count; ++i) {
      const float t = static_cast<float>(i) / static_cast<float>(point_count - 1);
      points.emplace_back(cubic_bezier_interpolation(p0, p1, p2, p3, t));
    }

    return *this;
  }

  CompoundCurveBuilder& CompoundCurveBuilder::relative_line_to(Vec2F d1)
  {
    assert(!points.empty());
    const Vec2F p = points.back();
    return line_to(p + d1);
  }

  CompoundCurveBuilder& CompoundCurveBuilder::relative_quadratic_bezier_curve_to(Vec2F d1, Vec2F d2, uint32_t point_count)
  {
    assert(!points.empty());
    const Vec2F p = points.back();
    return quadratic_bezier_curve_to(p + d1, p + d2, point_count);
  }

  CompoundCurveBuilder& CompoundCurveBuilder::relative_cubic_bezier_curve_to(Vec2F d1, Vec2F d2, Vec2F d3, uint32_t point_count)
  {
    assert(!points.empty());
    const Vec2F p = points.back();
    return cubic_bezier_curve_to(p + d1, p + d2, p + d3, point_count);
  }

  void CompoundCurveBuilder::close()
  {
    type = PolylineType::Loop;
  }

  void CompoundCurveBuilder::clear(Vec2F origin)
  {
    points.clear();
    points.emplace_back(origin);
    type = PolylineType::Chain;
  }

  CurveBuffer CurveBuffer::make_line(Vec2F p0, Vec2F p1)
  {
    CurveBuffer buffer;
    buffer.points.emplace_back(p0);
    buffer.points.emplace_back(p1);
    return buffer;
  }

  CurveBuffer CurveBuffer::make_compound_curve(Span<const Vec2F> points, PolylineType type)
  {
    CurveBuffer buffer;
    buffer.points.assign(points.begin(), points.end());
    buffer.type = type;
    return buffer;
  }

  CurveBuffer CurveBuffer::make_quadratic_bezier(Vec2F p0, Vec2F p1, Vec2F p2, uint32_t point_count)
  {
    assert(point_count >= 2);
    CurveBuffer buffer;

    for (uint32_t i = 1; i < point_count; ++i) {
      const float t = static_cast<float>(i) / static_cast<float>(point_count - 1);
      buffer.points.emplace_back(quadratic_bezier_interpolation(p0, p1, p2, t));
    }

    return buffer;
  }

  CurveBuffer CurveBuffer::make_cubic_bezier(Vec2F p0, Vec2F p1, Vec2F p2, Vec2F p3, uint32_t point_count)
  {
    assert(point_count >= 2);
    CurveBuffer buffer;

    for (uint32_t i = 1; i < point_count; ++i) {
      const float t = static_cast<float>(i) / static_cast<float>(point_count - 1);
      buffer.points.emplace_back(cubic_bezier_interpolation(p0, p1, p2, p3, t));
    }

    return buffer;
  }

  CurveBuffer CurveBuffer::make_cattmull_rom_spline(Span<const Vec2F> points, PolylineType type, CattmullRomType spline_type, uint32_t point_count)
  {
    assert(point_count >= 2);
    const PolylineView polyline_view = { type, points };

    auto size = points.size();
    const Vec2F prev_extension_point = 2 * points[0] - points[1];               // == p_0 - (p_1 - p_0);
    const Vec2F next_extension_point = 2 * points[size - 1] - points[size - 2]; // = p_{n-1} - (p_{n-2} - p_{n-1})

    CurveBuffer buffer;

    for (std::size_t i = 0; i < size - 1; ++i) {
      const Vec2F p0 = polyline_view.has_prev(i) ? polyline_view.prev_point(i) : prev_extension_point;
      const Vec2F p1 = points[i];
      const Vec2F p2 = points[i + 1];
      const Vec2F p3 = polyline_view.has_next(i + 1) ? polyline_view.next_point(i + 1) : next_extension_point;

      const float t0 = 0.0f;
      const float t1 = t0 + cattmull_rom_time_difference(p0, p1, spline_type);
      const float t2 = t1 + cattmull_rom_time_difference(p1, p2, spline_type);
      const float t3 = t2 + cattmull_rom_time_difference(p2, p3, spline_type);

      for (uint32_t j = 0; j < point_count; ++j) {
        const float t = t1 + ((t2 - t1) * static_cast<float>(j) / static_cast<float>(point_count));
        buffer.points.emplace_back(cattmull_rom_interpolation(p0, t0, p1, t1, p2, t2, p3, t3, t));
      }
    }

    buffer.type = type;
    return buffer;
  }

}
