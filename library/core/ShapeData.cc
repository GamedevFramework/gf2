// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/core/ShapeData.h>
// clang-format on

#include <algorithm>

namespace gf {

  ShapeData ShapeData::make_rectangle(Vec2F size)
  {
    return make_rectangle(RectF::from_position_size(-0.5f * size, size));
  }

  ShapeData ShapeData::make_rectangle(RectF rectangle)
  {
    ShapeData data = {};
    data.points = {
      rectangle.position_at(Orientation::NorthWest),
      rectangle.position_at(Orientation::NorthEast),
      rectangle.position_at(Orientation::SouthEast),
      rectangle.position_at(Orientation::SouthWest),
    };
    return data;
  }

  ShapeData ShapeData::make_rounded_rectangle(Vec2F size, float radius, uint32_t corner_point_count)
  {
    return make_rounded_rectangle(RectF::from_position_size(-0.5f * size, size), radius, corner_point_count);
  }

  ShapeData ShapeData::make_rounded_rectangle(RectF rectangle, float radius, uint32_t corner_point_count)
  {
    if (corner_point_count < 2) {
      return make_rectangle(rectangle);
    }

    auto size = rectangle.size();
    radius = std::min({ radius, size.h / 2.0f, size.w / 2.0f });

    ShapeData data = {};

    auto make_corner = [=, &data](Vec2F center, uint32_t corner) {
      for (uint32_t i = 0; i < corner_point_count; ++i) {
        const float angle = Pi2 * static_cast<float>(i) / static_cast<float>(corner_point_count - 1) + static_cast<float>(corner) * Pi2 + Pi;
        data.points.push_back(center + radius * unit(angle));
      }
    };

    make_corner(rectangle.position_at(Orientation::NorthWest) + vec(+radius, +radius), 0);
    make_corner(rectangle.position_at(Orientation::NorthEast) + vec(-radius, +radius), 1);
    make_corner(rectangle.position_at(Orientation::SouthEast) + vec(-radius, -radius), 2);
    make_corner(rectangle.position_at(Orientation::SouthWest) + vec(+radius, -radius), 3);

    return data;
  }

  ShapeData ShapeData::make_rhombus(Vec2F size)
  {
    return make_rhombus(RectF::from_position_size(-0.5f * size, size));
  }

  ShapeData ShapeData::make_rhombus(RectF rectangle)
  {
    ShapeData data = {};
    data.points = {
      rectangle.position_at(Orientation::North),
      rectangle.position_at(Orientation::East),
      rectangle.position_at(Orientation::South),
      rectangle.position_at(Orientation::West),
    };
    return data;
  }

  ShapeData ShapeData::make_circle(float radius, uint32_t point_count)
  {
    return make_circle(CircF::from_radius(radius), point_count);
  }

  ShapeData ShapeData::make_circle(CircF circle, uint32_t point_count)
  {
    ShapeData data = {};

    if (point_count < 3) {
      point_count = 3;
    }

    for (uint32_t i = 0; i < point_count; ++i) {
      const float angle = static_cast<float>(i) * 2.0f * Pi / static_cast<float>(point_count) - Pi2;
      data.points.push_back(circle.center + circle.radius * unit(angle));
    }

    return data;
  }

  ShapeData ShapeData::make_pie(float radius, float from_angle, float to_angle, uint32_t point_count)
  {
    return make_pie(CircF::from_radius(radius), from_angle, to_angle, point_count);
  }

  ShapeData ShapeData::make_pie(CircF circle, float from_angle, float to_angle, uint32_t point_count)
  {
    while (to_angle < from_angle) {
      to_angle += 2 * Pi;
    }

    if (point_count < 3) {
      point_count = 3;
    }

    ShapeData data = {};
    data.points.emplace_back(0.0f, 0.0f);

    const float section = to_angle - from_angle;

    for (uint32_t i = 0; i < point_count - 1; ++i) {
      const float angle = from_angle + static_cast<float>(i) * section / static_cast<float>(point_count - 1);
      data.points.push_back(circle.center + circle.radius * unit(angle));
    }

    return data;
  }

  ShapeData ShapeData::make_star(float max_radius, float min_radius, uint32_t branches)
  {
    return make_star(CircF::from_radius(max_radius), min_radius, branches);
  }

  ShapeData ShapeData::make_star(CircF max_circle, float min_radius, uint32_t branches)
  {
    ShapeData data = {};

    for (uint32_t i = 0; i < 2 * branches; ++i) {
      const float angle = static_cast<float>(i) * Pi / static_cast<float>(branches) - Pi2;
      const float radius = (i % 2 == 0) ? max_circle.radius : min_radius;
      data.points.push_back(max_circle.center + radius * unit(angle));
    }

    return data;
  }

}
