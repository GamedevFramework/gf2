// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Curve.h>
// clang-format on

#include <cassert>

#include <algorithm>
#include <iterator>

#include <gf2/core/Polyline.h>
#include <gf2/core/Range.h>

#include <gf2/graphics/Vertex.h>

namespace gf {

  namespace {
    Vec2F compute_normal(Vec2F prev, Vec2F curr, Vec2F next)
    {
      Vec2F normal_prev = normalize(perp(curr - prev));
      Vec2F normal_next = normalize(perp(next - curr));

      float factor = 1.0f + dot(normal_prev, normal_next);
      return (normal_prev + normal_next) / factor;
    }

    template<typename Geometry>
    void compute_curve_geometry(Geometry& geometry, const CurveData& data, float half_width, Color color)
    {
      assert(data.points.size() >= 2);

      auto compute_vertex = [color](Vec2F location) -> Vertex { return { location, { 0.0f, 0.0f }, color }; };

      // first point

      Vec2F curr = data.points[0];
      Vec2F next = data.points[1];

      if (data.type == PolylineType::Loop) {
        const Vec2F prev = data.points.back();
        const Vec2F normal = compute_normal(prev, curr, next);
        geometry.vertices.emplace_back(compute_vertex(curr - normal * half_width));
        geometry.vertices.emplace_back(compute_vertex(curr + normal * half_width));
      } else {
        const Vec2F normal = normalize(perp(next - curr));
        geometry.vertices.emplace_back(compute_vertex(curr - normal * half_width));
        geometry.vertices.emplace_back(compute_vertex(curr + normal * half_width));
      }

      // middle points

      for (std::size_t i = 2; i < data.points.size(); ++i) {
        const Vec2F prev = curr;
        curr = next;
        next = data.points[i];
        const Vec2F normal = compute_normal(prev, curr, next);
        geometry.vertices.emplace_back(compute_vertex(curr - normal * half_width));
        geometry.vertices.emplace_back(compute_vertex(curr + normal * half_width));
      }

      // last point

      const Vec2F prev = curr;
      curr = data.points.back();

      if (data.type == PolylineType::Loop) {
        next = data.points.front();
        const Vec2F normal = compute_normal(prev, curr, next);
        geometry.vertices.emplace_back(compute_vertex(curr - normal * half_width));
        geometry.vertices.emplace_back(compute_vertex(curr + normal * half_width));
      } else {
        const Vec2F normal = normalize(perp(curr - prev));
        geometry.vertices.emplace_back(compute_vertex(curr - normal * half_width));
        geometry.vertices.emplace_back(compute_vertex(curr + normal * half_width));
      }

      // simulate triangle strip

      std::size_t triangle_count = geometry.vertices.size();

      if (data.type == PolylineType::Chain) {
        triangle_count -= 2;
      }

      for (auto i : gf::index_range(triangle_count)) {
        geometry.indices.push_back(static_cast<uint16_t>(i));
        geometry.indices.push_back(static_cast<uint16_t>((i + 1) % geometry.vertices.size()));
        geometry.indices.push_back(static_cast<uint16_t>((i + 2) % geometry.vertices.size()));
      }
    }

    struct RawInteriorCurveGeometry {
      std::vector<Vertex> vertices;
      std::vector<uint16_t> indices;
      RectF bounds = {};
    };

    RawInteriorCurveGeometry compute_interior_curve_geometry(const CurveData& data)
    {
      RawInteriorCurveGeometry geometry;
      compute_curve_geometry(geometry, data, data.thickness / 2.0f, data.color);

      geometry.bounds = RectF::from_center_size(geometry.vertices.front().location, { 0.0f, 0.0f });

      for (const auto& vertex : geometry.vertices) {
        geometry.bounds.extend_to(vertex.location);
      }

      return geometry;
    }

    struct RawOutlineCurveGeometry {
      std::vector<Vertex> vertices;
      std::vector<uint16_t> indices;
    };

    RawOutlineCurveGeometry compute_outline_curve_geometry(const CurveData& data)
    {
      RawOutlineCurveGeometry geometry;
      compute_curve_geometry(geometry, data, data.thickness / 2.0f + data.outline_thickness, data.outline_color);
      return geometry;
    }

    struct RawCurveGeometry {
      std::vector<Vertex> vertices;
      std::vector<uint16_t> indices;

      template<typename T>
      void merge_with(const T& other) {
        const std::size_t offset = vertices.size();
        vertices.insert(vertices.end(), other.vertices.begin(), other.vertices.end());
        std::transform(other.indices.begin(), other.indices.end(), std::back_inserter(indices), [offset](uint16_t index) {
          return index + offset;
        });
      }
    };

  }

  /*
   * Curve
   */

  Curve::Curve(const CurveData& data, RenderManager* render_manager)
  {
    auto raw_interior = compute_interior_curve_geometry(data);
    m_vertices = Buffer(BufferType::Device, BufferUsage::Vertex, raw_interior.vertices.data(), raw_interior.vertices.size(), render_manager);
    m_indices = Buffer(BufferType::Device, BufferUsage::Index, raw_interior.indices.data(), raw_interior.indices.size(), render_manager);
    m_bounds = raw_interior.bounds;

    if (data.outline_thickness > 0.0f) {
      auto raw_outline = compute_outline_curve_geometry(data);
      m_outline_vertices = Buffer(BufferType::Device, BufferUsage::Vertex, raw_outline.vertices.data(), raw_outline.vertices.size(), render_manager);
      m_outline_indices = Buffer(BufferType::Device, BufferUsage::Index, raw_outline.indices.data(), raw_outline.indices.size(), render_manager);
    }
  }

  CurveGeometry Curve::geometry() const
  {
    CurveGeometry geometry;

    RenderGeometry curve;
    curve.texture = nullptr;
    curve.vertices = &m_vertices;
    curve.indices = &m_indices;
    curve.count = m_indices.count();

    geometry.curve = curve;

    if (m_outline_vertices.has_value()) {
      assert(m_outline_indices.has_value());

      RenderGeometry outline;
      outline.texture = nullptr;
      outline.vertices = &m_outline_vertices.value();
      outline.indices = &m_outline_indices.value();
      outline.count = m_outline_indices.value().count();

      geometry.outline = outline;
    }

    return geometry;
  }

  /*
   * CurveGroup
   */

  CurveGroup::CurveGroup(const CurveGroupData& data, RenderManager* render_manager)
  {
    update(data, render_manager);
  }

  void CurveGroup::update(const CurveGroupData& data, RenderManager* render_manager)
  {
    RawCurveGeometry geometry;

    for (const auto& curve : data.curves) {
      auto raw_interior = compute_interior_curve_geometry(curve);
      geometry.merge_with(raw_interior);

      if (curve.outline_thickness > 0.0f) {
        auto raw_outline = compute_outline_curve_geometry(curve);
        geometry.merge_with(raw_outline);
      }
    }

    m_current_buffer = (m_current_buffer + 1) % FramesInFlight;
    auto& current_vertices = m_vertices[m_current_buffer]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    auto& current_indices = m_indices[m_current_buffer]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

    if (geometry.vertices.size() > current_vertices.size()) {
      current_vertices = Buffer(BufferType::Host, BufferUsage::Vertex, geometry.vertices.data(), geometry.vertices.size(), render_manager);
      current_vertices.set_debug_name("[gf2] Curve Group Vertex Buffer #" + std::to_string(m_current_buffer));
    } else {
      current_vertices.update(geometry.vertices.data(), geometry.vertices.size(), render_manager);
    }

    if (geometry.indices.size() > current_indices.size()) {
      current_indices = Buffer(BufferType::Host, BufferUsage::Index, geometry.indices.data(), geometry.indices.size(), render_manager);
      current_indices.set_debug_name("[gf2] Curve Group Index Buffer #" + std::to_string(m_current_buffer));
    } else {
      current_indices.update(geometry.indices.data(), geometry.indices.size(), render_manager);
    }
  }

  RenderGeometry CurveGroup::geometry() const
  {
    RenderGeometry geometry;
    geometry.vertices = &m_vertices[m_current_buffer]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    geometry.indices = &m_indices[m_current_buffer]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    geometry.count = m_indices[m_current_buffer].count(); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    return geometry;
  }
}
