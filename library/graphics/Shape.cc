// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Shape.h>
// clang-format on

#include <cassert>
#include <cstdint>

#include <gf2/core/Polygon.h>
#include <gf2/core/Range.h>

#include <gf2/graphics/Vertex.h>

namespace gf {

  namespace {
    struct RawInteriorShapeGeometry {
      std::vector<Vertex> vertices;
      std::vector<uint16_t> indices;
      RectF bounds = {};
      Vec2F center = {};
    };

    RawInteriorShapeGeometry compute_interior_shape_geometry(const ShapeData& data)
    {
      assert(data.points.size() > 2);

      RawInteriorShapeGeometry geometry;

      geometry.center = { 0.0f, 0.0f };

      for (auto point : data.points) {
        geometry.center += point;
      }

      geometry.center /= data.points.size();
      assert(contains(data.points, geometry.center));

      Vec2F min = data.points[0];
      Vec2F max = data.points[0];

      Vertex center_vertex;
      center_vertex.location = geometry.center;
      center_vertex.color = data.color;

      geometry.vertices.push_back(center_vertex);

      for (auto point : data.points) {
        Vertex vertex;
        vertex.location = point;
        vertex.color = data.color;

        min = gf::min(min, point);
        max = gf::max(max, point);

        geometry.vertices.push_back(vertex);
      }

      // simulate triangle fan
      for (auto i : gf::index_range(data.points.size())) {
        geometry.indices.push_back(0);
        geometry.indices.push_back(static_cast<uint16_t>(1 + i));
        geometry.indices.push_back(static_cast<uint16_t>(1 + (i + 1) % data.points.size()));
      }

      geometry.bounds = RectF::from_min_max(min, max);

      for (auto& vertex : geometry.vertices) {
        const Vec2F ratio = (vertex.location - geometry.bounds.position()) / geometry.bounds.size();
        vertex.tex_coords = data.texture_region.position() + data.texture_region.size() * ratio;
      }

      return geometry;
    }

    struct RawOutlineShapeGeometry {
      std::vector<Vertex> vertices;
      std::vector<uint16_t> indices;
      RectF bounds = {};
    };

    RawOutlineShapeGeometry compute_outline_shape_geometry(const ShapeData& data, Vec2F center)
    {
      RawOutlineShapeGeometry geometry;

      for (auto i : gf::index_range(data.points.size())) {
        const Vec2F prev = data.points[i];
        const Vec2F curr = data.points[(i + 1) % data.points.size()];
        const Vec2F next = data.points[(i + 2) % data.points.size()];

        Vec2F normal_prev = gf::normalize(gf::perp(curr - prev));
        Vec2F normal_next = gf::normalize(gf::perp(next - curr));

        if (gf::dot(normal_prev, center - curr) > 0.0f) {
          normal_prev = -normal_prev;
        }

        if (gf::dot(normal_next, center - curr) > 0.0f) {
          normal_next = -normal_next;
        }

        const float factor = 1.0f + gf::dot(normal_prev, normal_next);
        const Vec2F normal = (normal_prev + normal_next) / factor;

        Vertex vertices[2];
        vertices[0].location = curr;
        vertices[0].color = data.outline_color;
        vertices[1].location = curr + normal * data.outline_thickness;
        vertices[1].color = data.outline_color;

        geometry.vertices.push_back(vertices[0]);
        geometry.vertices.push_back(vertices[1]);
      }

      // simulate triangle strip
      for (auto i : gf::index_range(geometry.vertices.size())) {
        geometry.indices.push_back(static_cast<uint16_t>(i));
        geometry.indices.push_back(static_cast<uint16_t>((i + 1) % geometry.vertices.size()));
        geometry.indices.push_back(static_cast<uint16_t>((i + 2) % geometry.vertices.size()));
      }

      return geometry;
    }

  }

  Shape::Shape(const Texture* texture, const ShapeData& data, Renderer* renderer)
  : m_texture(texture)
  {
    auto raw_interior = compute_interior_shape_geometry(data);
    m_vertices = Buffer(BufferType::Device, BufferUsage::Vertex, raw_interior.vertices.data(), raw_interior.vertices.size(), renderer);
    m_indices = Buffer(BufferType::Device, BufferUsage::Index, raw_interior.indices.data(), raw_interior.indices.size(), renderer);
    m_bounds = raw_interior.bounds;

    if (data.outline_thickness > 0.0f) {
      auto raw_outline = compute_outline_shape_geometry(data, raw_interior.center);
      m_outline_vertices = Buffer(BufferType::Device, BufferUsage::Vertex, raw_outline.vertices.data(), raw_outline.vertices.size(), renderer);
      m_outline_indices = Buffer(BufferType::Device, BufferUsage::Index, raw_outline.indices.data(), raw_outline.indices.size(), renderer);
    }
  }

  ShapeGeometry Shape::geometry() const
  {
    ShapeGeometry geometry;

    RenderGeometry shape;
    shape.texture = m_texture;
    shape.vertices = &m_vertices;
    shape.indices = &m_indices;
    shape.count = m_indices.count();

    geometry.shape = shape;

    if (m_outline_vertices.has_value()) {
      assert(m_outline_indices.has_value());

      RenderGeometry outline;
      outline.pipeline = RenderPipelineType::Text;
      outline.texture = nullptr;
      outline.vertices = &m_outline_vertices.value();
      outline.indices = &m_outline_indices.value();
      outline.count = m_outline_indices.value().count();

      geometry.outline = outline;
    }

    return geometry;
  }

}
