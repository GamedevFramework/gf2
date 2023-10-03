// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SHAPE_DATA_H
#define GF_SHAPE_DATA_H

#include <cstdint>

#include <filesystem>
#include <vector>

#include "Circ.h"
#include "Color.h"
#include "CoreApi.h"
#include "Rect.h"
#include "TypeTraits.h"
#include "Vec2.h"

namespace gf {

  struct GF_CORE_API ShapeData {
    static ShapeData make_rectangle(Vec2F size);
    static ShapeData make_rectangle(RectF rectangle);
    static ShapeData make_rounded_rectangle(Vec2F size, float radius, uint32_t corner_point_count = 8);
    static ShapeData make_rounded_rectangle(RectF rectangle, float radius, uint32_t corner_point_count = 8);
    static ShapeData make_rhombus(Vec2F size);
    static ShapeData make_rhombus(RectF rectangle);
    static ShapeData make_circle(float radius, uint32_t point_count = 32);
    static ShapeData make_circle(CircF circle, uint32_t point_count = 32);
    static ShapeData make_pie(float radius, float from_angle, float to_angle, uint32_t point_count = 32);
    static ShapeData make_pie(CircF circle, float from_angle, float to_angle, uint32_t point_count = 32);
    static ShapeData make_star(float max_radius, float min_radius, uint32_t branches);
    static ShapeData make_star(CircF max_circle, float min_radius, uint32_t branches);

    std::vector<Vec2F> points;
    Color color = Black;
    float outline_thickness = 0.0f;
    Color outline_color = White;
    RectF texture_region = RectF::from_size({ 1.0f, 1.0f });
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ShapeData, Archive>& data)
  {
    return ar | data.points | data.color | data.outline_thickness | data.outline_color | data.texture_region;
  }

  struct GF_CORE_API ShapeResource {
    std::filesystem::path texture;
    ShapeData data;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ShapeResource, Archive>& resource)
  {
    return ar | resource.texture | resource.data;
  }

}

#endif // GF_SHAPE_DATA_H
