// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Positioning.h>

#include <algorithm>

namespace gf {

  Positioning::Positioning(Vec2I surface_size)
  : m_surface_size(surface_size)
  {
  }

  Vec2F Positioning::center() const
  {
    return m_surface_size * 0.5f;
  }

  Vec2F Positioning::relative_point(Vec2F percent) const
  {
    return m_surface_size * percent;
  }

  Vec2F Positioning::relative_size(Vec2F percent) const
  {
    return m_surface_size * percent;
  }

  float Positioning::relative_thickness(float percent) const
  {
    auto size = relative_size({ percent, percent });
    return std::max(size.x, size.y);
  }

  float Positioning::character_size(float percent) const
  {
    return static_cast<float>(m_surface_size.h) * percent;
  }

  namespace {

    float compute_x_distance(int w, float x, Orientation orientation)
    {
      switch (orientation) {
        case Orientation::NorthWest:
        case Orientation::West:
        case Orientation::SouthWest:
          return x;
        case Orientation::North:
        case Orientation::Center:
        case Orientation::South:
          return static_cast<float>(w) * 0.5f;
        case Orientation::NorthEast:
        case Orientation::East:
        case Orientation::SouthEast:
          return static_cast<float>(w) - x;
      }

      assert(false);
      return 0.0f;
    }

    float compute_y_distance(int h, float y, Orientation orientation)
    {
      switch (orientation) {
        case Orientation::NorthWest:
        case Orientation::North:
        case Orientation::NorthEast:
          return y;
        case Orientation::West:
        case Orientation::Center:
        case Orientation::East:
          return static_cast<float>(h) * 0.5f;
        case Orientation::SouthWest:
        case Orientation::South:
        case Orientation::SouthEast:
          return static_cast<float>(h) - y;
      }

      assert(false);
      return 0.0f;
    }

  }

  Vec2F Positioning::absolute_point(Vec2F distance, Orientation orientation) const
  {
    return { compute_x_distance(m_surface_size.w, distance.x, orientation), compute_y_distance(m_surface_size.h, distance.y, orientation) };
  }

}
