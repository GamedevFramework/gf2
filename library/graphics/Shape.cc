// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Shape.h>

#include <cassert>
#include <cstdint>

#include <gf2/core/Polygon.h>
#include <gf2/core/Range.h>
#include <gf2/core/ResourceManager.h>

#include <gf2/graphics/GpuTexture.h>
#include <gf2/graphics/RawGeometry.h>
#include <gf2/graphics/Vertex.h>

namespace gf {

  namespace {
    struct RawInteriorShapeGeometry : RawGeometry {
      RectF bounds = {};
      Vec2F center = {};
    };

    RawInteriorShapeGeometry compute_interior_shape_geometry(const ShapeBuffer& buffer)
    {
      assert(buffer.points.size() > 2);

      RawInteriorShapeGeometry geometry;

      geometry.center = { 0.0f, 0.0f };

      for (auto point : buffer.points) {
        geometry.center += point;
      }

      geometry.center /= buffer.points.size();
      assert(contains(buffer.points, geometry.center));

      Vec2F min = buffer.points[0];
      Vec2F max = buffer.points[0];

      const Color linear_color = srgb_to_linear(buffer.color);

      Vertex center_vertex;
      center_vertex.location = geometry.center;
      center_vertex.color = linear_color;

      geometry.vertices.push_back(center_vertex);

      for (auto point : buffer.points) {
        Vertex vertex;
        vertex.location = point;
        vertex.color = linear_color;

        min = gf::min(min, point);
        max = gf::max(max, point);

        geometry.vertices.push_back(vertex);
      }

      // simulate triangle fan
      for (auto i : gf::index_range(buffer.points.size())) {
        geometry.indices.push_back(0);
        geometry.indices.push_back(static_cast<uint16_t>(1 + i));
        geometry.indices.push_back(static_cast<uint16_t>(1 + ((i + 1) % buffer.points.size())));
      }

      geometry.bounds = RectF::from_min_max(min, max);

      for (auto& vertex : geometry.vertices) {
        const Vec2F ratio = (vertex.location - geometry.bounds.position()) / geometry.bounds.size();
        vertex.tex_coords = buffer.texture_region.position() + buffer.texture_region.size() * ratio;
      }

      return geometry;
    }

    using RawOutlineShapeGeometry = RawGeometry;

    RawOutlineShapeGeometry compute_outline_shape_geometry(const ShapeBuffer& buffer, Vec2F center)
    {
      RawOutlineShapeGeometry geometry;
      const Color linear_outline_color = gf::srgb_to_linear(buffer.outline_color);

      for (auto i : gf::index_range(buffer.points.size())) {
        const Vec2F prev = buffer.points[i];
        const Vec2F curr = buffer.points[(i + 1) % buffer.points.size()];
        const Vec2F next = buffer.points[(i + 2) % buffer.points.size()];

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
        vertices[0].color = linear_outline_color;
        vertices[1].location = curr + normal * buffer.outline_thickness;
        vertices[1].color = linear_outline_color;

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

    using RawShapeGeometry = RawGeometry;

    const GpuTexture* load_resource(const ShapeResource& resource, ResourceManager* resource_manager)
    {
      if (resource.texture.empty()) {
        return nullptr;
      }

      return resource_manager->get<GpuTexture>(resource.texture);
    }
  }

  Shape::Shape(const GpuTexture* texture, const ShapeBuffer& buffer, RenderManager* render_manager)
  : m_texture(texture)
  {
    auto raw_interior = compute_interior_shape_geometry(buffer);
    m_vertices = Buffer(BufferType::Device, BufferUsage::Vertex, raw_interior.vertices.data(), raw_interior.vertices.size(), render_manager);
    m_indices = Buffer(BufferType::Device, BufferUsage::Index, raw_interior.indices.data(), raw_interior.indices.size(), render_manager);
    m_bounds = raw_interior.bounds;

    if (buffer.outline_thickness > 0.0f) {
      auto raw_outline = compute_outline_shape_geometry(buffer, raw_interior.center);
      m_outline_vertices = Buffer(BufferType::Device, BufferUsage::Vertex, raw_outline.vertices.data(), raw_outline.vertices.size(), render_manager);
      m_outline_indices = Buffer(BufferType::Device, BufferUsage::Index, raw_outline.indices.data(), raw_outline.indices.size(), render_manager);
    }
  }

  Shape::Shape(const ShapeResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : Shape(load_resource(resource, resource_manager), resource.buffer, render_manager)
  {
  }

  ShapeGeometry Shape::geometry() const
  {
    ShapeGeometry geometry;

    RenderGeometry shape;
    shape.texture = m_texture;
    shape.vertices = &m_vertices;
    shape.indices = &m_indices;
    shape.size = m_indices.size();

    geometry.shape = shape;

    if (m_outline_vertices.has_value()) {
      assert(m_outline_indices.has_value());

      RenderGeometry outline;
      outline.texture = nullptr;
      outline.vertices = &m_outline_vertices.value();
      outline.indices = &m_outline_indices.value();
      outline.size = m_outline_indices.value().size();

      geometry.outline = outline;
    }

    return geometry;
  }

  /*
   * ShapeGroup
   */

  ShapeGroup::ShapeGroup()
  : m_vertices(BufferType::Host, BufferUsage::Vertex)
  , m_indices(BufferType::Host, BufferUsage::Index)
  {
    m_vertices.set_debug_name("[gf2] Shape Group Vertex Buffer");
    m_indices.set_debug_name("[gf2] Shape Group Index Buffer");
  }

  ShapeGroup::ShapeGroup(const ShapeGroupBuffer& buffer, RenderManager* render_manager)
  : ShapeGroup()
  {
    update(buffer, render_manager);
  }

  void ShapeGroup::update(const ShapeGroupBuffer& buffer, RenderManager* render_manager)
  {
    RawShapeGeometry geometry;

    for (const auto& shape : buffer.shapes) {
      auto raw_interior = compute_interior_shape_geometry(shape);
      geometry.merge_with(raw_interior);

      if (shape.outline_thickness > 0.0f) {
        auto raw_outline = compute_outline_shape_geometry(shape, raw_interior.center);
        geometry.merge_with(raw_outline);
      }
    }

    m_vertices.update(geometry.vertices.data(), geometry.vertices.size(), render_manager);
    m_indices.update(geometry.indices.data(), geometry.indices.size(), render_manager);
    m_bounds = geometry.compute_bounds();
  }

  RenderGeometry ShapeGroup::geometry() const
  {
    RenderGeometry geometry = {};
    geometry.vertices = &m_vertices.buffer();
    geometry.indices = &m_indices.buffer();
    geometry.size = m_indices.buffer().size();
    return geometry;
  }

}
