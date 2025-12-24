// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SHAPE_BUFFER_H
#define GF_SHAPE_BUFFER_H

#include <cstdint>

#include <filesystem>
#include <vector>

#include "Circ.h"
#include "Color.h"
#include "CoreApi.h"
#include "Geometry.h"
#include "Rect.h"
#include "Span.h"
#include "TypeTraits.h"
#include "Vec2.h"

namespace gf {

  struct GF_CORE_API ShapeBuffer {
    static ShapeBuffer make_rectangle(Vec2F size);
    static ShapeBuffer make_rectangle(RectF rectangle);
    static ShapeBuffer make_rounded_rectangle(Vec2F size, float radius, uint32_t corner_point_count = 8);
    static ShapeBuffer make_rounded_rectangle(RectF rectangle, float radius, uint32_t corner_point_count = 8);
    static ShapeBuffer make_rhombus(Vec2F size);
    static ShapeBuffer make_rhombus(RectF rectangle);
    static ShapeBuffer make_circle(float radius, uint32_t point_count = 32);
    static ShapeBuffer make_circle(CircF circle, uint32_t point_count = 32);
    static ShapeBuffer make_capsule(Vec2F center0, Vec2F center1, float radius, uint32_t point_count = 32);
    static ShapeBuffer make_capsule(CapsuleF capsule, uint32_t point_count = 32);
    static ShapeBuffer make_pie(float radius, float from_angle, float to_angle, uint32_t point_count = 32);
    static ShapeBuffer make_pie(CircF circle, float from_angle, float to_angle, uint32_t point_count = 32);
    static ShapeBuffer make_star(float max_radius, float min_radius, uint32_t branches);
    static ShapeBuffer make_star(CircF max_circle, float min_radius, uint32_t branches);
    static ShapeBuffer make_polygon(Span<const Vec2F> contour);

    std::vector<Vec2F> points;
    Color color = Black;
    float outline_thickness = 0.0f;
    Color outline_color = White;
    RectF texture_region = RectF::from_size({ 1.0f, 1.0f });
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ShapeBuffer, Archive>& buffer)
  {
    return ar | buffer.points | buffer.color | buffer.outline_thickness | buffer.outline_color | buffer.texture_region;
  }

  struct GF_CORE_API ShapeResource {
    std::filesystem::path texture;
    ShapeBuffer buffer;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ShapeResource, Archive>& resource)
  {
    return ar | resource.texture | resource.buffer;
  }

  struct GF_CORE_API ShapeGroupBuffer {
    std::vector<ShapeBuffer> shapes;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ShapeGroupBuffer, Archive>& data)
  {
    return ar | data.shapes;
  }

}

#endif // GF_SHAPE_BUFFER_H
