// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/GridVisibility.h>

#include <limits>

#include <gf2/core/Geometry.h>
#include <gf2/core/GridMap.h>

#include "gf2/core/Vec2.h"

namespace gf {

  // all implementations are adapted from http://www.adammil.net/blog/v125_Roguelike_Vision_Algorithms.html

  namespace {

    Vec2I transform_octant(Vec2I origin, Vec2I relative, int octant)
    {
      switch (octant) {
          // clang-format off
        case 0: origin.x += relative.x; origin.y -= relative.y; break;
        case 1: origin.x += relative.y; origin.y -= relative.x; break;
        case 2: origin.x -= relative.y; origin.y -= relative.x; break;
        case 3: origin.x -= relative.x; origin.y -= relative.y; break;
        case 4: origin.x -= relative.x; origin.y += relative.y; break;
        case 5: origin.x -= relative.y; origin.y += relative.x; break;
        case 6: origin.x += relative.y; origin.y += relative.x; break;
        case 7: origin.x += relative.x; origin.y += relative.y; break;
        // clang-format on
        default:
          assert(false);
          break;
      }

      return origin;
    }

    enum class Opacity : uint8_t {
      Unknown,
      Opaque,
      Transparent,
    };

  }

  /*
   * RayCastVisibility
   */

  RayCastVisibility::RayCastVisibility(GridMap* map, Flags<CellProperty> properties)
  : m_map(map)
  , m_properties(properties)
  {
  }

  void RayCastVisibility::compute_visibility(Vec2I origin, int range_limit) const
  {
    if (m_map == nullptr) {
      return;
    }

    m_map->add_properties(origin, m_properties);

    if (range_limit == 0) {
      return;
    }

    RectI area = RectI::from_size(m_map->size());

    if (range_limit >= 0) {
      RectI limited = RectI::from_position_size(origin - range_limit, vec(1, 1) * (2 * range_limit + 1));

      auto maybe_intersection = area.intersection(limited);

      if (!maybe_intersection) {
        return;
      }

      area = *maybe_intersection;
    }

    auto min = area.min();
    auto max = area.max() - 1;

    for (int x = min.x; x < max.x; ++x) {
      trace_line(origin, { x, min.y - 1 }, range_limit);
      trace_line(origin, { x, max.y }, range_limit);
    }

    for (int y = min.y; y < max.y; ++y) {
      trace_line(origin, { min.x - 1, y }, range_limit);
      trace_line(origin, { max.x, y }, range_limit);
    }
  }

  void RayCastVisibility::trace_line(Vec2I origin, Vec2I target, int range_limit) const
  {
    const int square_range_limit = square(range_limit);
    BresenhamAlgorithm bresenham(origin, target);

    for (;;) {
      if (auto maybe_current = bresenham.step(); maybe_current) {
        Vec2I current = *maybe_current;

        if (square_distance(origin, current) > square_range_limit) {
          break;
        }

        m_map->add_properties(current, m_properties);

        if (!m_map->transparent(current)) {
          break;
        }
      } else {
        break;
      }
    }
  }

  /*
   * ShadowCastVisibility
   */

  ShadowCastVisibility::ShadowCastVisibility(GridMap* map, Flags<CellProperty> properties)
  : m_map(map)
  , m_properties(properties)
  {
  }

  void ShadowCastVisibility::compute_visibility(Vec2I origin, int range_limit) const
  {
    if (m_map == nullptr) {
      return;
    }

    m_map->add_properties(origin, m_properties);

    for (int octant = 0; octant < 8; ++octant) {
      compute(octant, origin, range_limit, 1, { 1, 1 }, { 0, 1 });
    }
  }

  // NOLINTNEXTLINE(misc-no-recursion,readability-function-cognitive-complexity)
  void ShadowCastVisibility::compute(int octant, Vec2I origin, int range_limit, int x, details::Slope top, details::Slope bottom) const
  {
    const int x_limit = range_limit >= 0 ? range_limit : std::numeric_limits<int>::max();
    const int square_range_limit = square(range_limit);

    for (; x <= x_limit; ++x) {
      const int top_y = top.x == 1 ? x : ((x * 2 + 1) * top.y + top.x - 1) / (top.x * 2);
      const int bottom_y = bottom.y == 0 ? 0 : ((x * 2 - 1) * bottom.y + bottom.x) / (bottom.x * 2);

      Opacity was_opaque = Opacity::Unknown;

      for (int y = top_y; y >= bottom_y; --y) {
        const Vec2I position = transform_octant(origin, { x, y }, octant);
        const bool in_range = range_limit < 0 || square_distance(origin, position) <= square_range_limit;

        if (in_range) {
          m_map->add_properties(position, m_properties);
        }

        const bool is_opaque = !in_range || !m_map->transparent(position);

        if (x != x_limit) {
          if (is_opaque) {
            if (was_opaque == Opacity::Transparent) {
              details::Slope new_bottom = { y * 2 + 1, x * 2 - 1 };

              if (!in_range || y == bottom_y) {
                bottom = new_bottom;
                break;
              }

              compute(octant, origin, range_limit, x + 1, top, new_bottom);
            }

            was_opaque = Opacity::Opaque;
          } else {
            if (was_opaque == Opacity::Opaque) {
              top = { y * 2 + 1, x * 2 + 1 };
            }

            was_opaque = Opacity::Transparent;
          }
        }
      }

      if (was_opaque != Opacity::Transparent) {
        break;
      }
    }
  }

  /*
   * DiamondWallsVisibility
   */

  DiamondWallsVisibility::DiamondWallsVisibility(GridMap* map, Flags<CellProperty> properties)
  : m_map(map)
  , m_properties(properties)
  {
  }

  void DiamondWallsVisibility::compute_visibility(Vec2I origin, int range_limit) const
  {
    if (m_map == nullptr) {
      return;
    }

    m_map->add_properties(origin, m_properties);

    for (int octant = 0; octant < 8; ++octant) {
      compute(octant, origin, range_limit, 1, { 1, 1 }, { 0, 1 });
    }
  }

  // NOLINTNEXTLINE(misc-no-recursion,readability-function-cognitive-complexity)
  void DiamondWallsVisibility::compute(int octant, Vec2I origin, int range_limit, int x, details::Slope top, details::Slope bottom) const
  {
    const int x_limit = range_limit >= 0 ? range_limit : std::numeric_limits<int>::max();
    const int square_range_limit = square(range_limit);

    for (; x <= x_limit; ++x) {
      int top_y = 0;
      if (top.x == 1) {
        top_y = x;
      } else {
        top_y = ((x * 2 - 1) * top.y + top.x) / (top.x * 2);
        const int ay = (top_y * 2 + 1) * top.x;

        if (!transparent(octant, origin, { x, top_y })) {
          if (top >= details::slope(ay, x * 2)) {
            ++top_y;
          }
        } else {
          if (top > details::slope(ay, x * 2 + 1)) {
            ++top_y;
          }
        }
      }

      const int bottom_y = bottom.y == 0 ? 0 : ((x * 2 - 1) * bottom.y + bottom.x) / (bottom.x * 2);

      Opacity was_opaque = Opacity::Unknown;

      for (int y = top_y; y >= bottom_y; --y) {
        const Vec2I position = transform_octant(origin, { x, y }, octant);
        const bool in_range = range_limit < 0 || square_distance(origin, position) <= square_range_limit;

        if (in_range) {
          m_map->add_properties(position, m_properties);
        }

        bool is_opaque = !in_range || !m_map->transparent(position);

        if (is_opaque && ((y == top_y && top <= details::slope(y * 2 - 1, x * 2) && transparent(octant, origin, { x, y - 1 })) || (y == bottom_y && bottom >= details::slope(y * 2 + 1, x * 2) && transparent(octant, origin, { x, y + 1 })))) {
          is_opaque = false;
        }

        if (x != x_limit) {
          if (is_opaque) {
            if (was_opaque == Opacity::Transparent) {
              details::Slope new_bottom = { y * 2 + 1, x * 2 - 1 };

              if (!in_range || y == bottom_y) {
                bottom = new_bottom;
                break;
              }

              compute(octant, origin, range_limit, x + 1, top, new_bottom);
            }

            was_opaque = Opacity::Opaque;
          } else {
            if (was_opaque == Opacity::Opaque) {
              top = { y * 2 + 1, x * 2 + 1 };
            }

            was_opaque = Opacity::Transparent;
          }
        }
      }

      if (was_opaque != Opacity::Transparent) {
        break;
      }
    }
  }

  bool DiamondWallsVisibility::transparent(int octant, Vec2I origin, Vec2I local) const
  {
    return m_map->transparent(transform_octant(origin, local, octant));
  }

  /*
   * PermissiveVisibility
   */

  PermissiveVisibility::PermissiveVisibility(GridMap* map, Flags<CellProperty> properties)
  : m_map(map)
  , m_properties(properties)
  {
  }

  void PermissiveVisibility::compute_visibility(Vec2I origin, int range_limit) const
  {
    if (m_map == nullptr) {
      return;
    }

    compute_quadrant({ +1, +1 }, origin, range_limit);
    compute_quadrant({ -1, +1 }, origin, range_limit);
    compute_quadrant({ +1, -1 }, origin, range_limit);
    compute_quadrant({ -1, -1 }, origin, range_limit);
  }

  void PermissiveVisibility::compute_quadrant(Vec2I quadrant, Vec2I origin, int range_limit) const
  {
    static constexpr int Infinity = std::numeric_limits<int>::max();

    std::list<Field> active_fields;
    const Field initial_field(Line({ 1, 0 }, { 0, Infinity }), Line({ 0, 1 }, { Infinity, 0 }));
    active_fields.push_back(initial_field);

    Vec2I local = { 0, 0 };
    blocked(local, quadrant, origin, range_limit);

    for (int i = 1; i < Infinity; ++i) {
      auto current_field = active_fields.begin();

      for (int j = 0; j <= i; ++j) {
        local.x = i - j;
        local.y = j;
        current_field = visit_square(local, quadrant, origin, range_limit, current_field, active_fields);
      }
    }
  }

  bool PermissiveVisibility::blocked(Vec2I local, Vec2I quadrant, Vec2I origin, int range_limit) const
  {
    if (range_limit >= 0 && square_length(local) > square(range_limit)) {
      return true;
    }

    Vec2I position = origin + quadrant * local;
    m_map->add_properties(position, m_properties);
    return !m_map->transparent(position);
  }

  auto PermissiveVisibility::visit_square(Vec2I local, Vec2I quadrant, Vec2I origin, int range_limit, FieldIterator current_field, FieldList& active_fields) const -> FieldIterator
  {
    Vec2I top_left = { local.x, local.y + 1 };
    Vec2I bottom_right = { local.x + 1, local.y };

    while (current_field != active_fields.end() && current_field->steep.is_below_or_contains(bottom_right)) {
      ++current_field;
    }

    if (current_field == active_fields.end() || current_field->shallow.is_above_or_contains(top_left) || !blocked(local, quadrant, origin, range_limit)) {
      return current_field;
    }

    if (current_field->shallow.is_above(bottom_right) && current_field->steep.is_below(top_left)) {
      return active_fields.erase(current_field);
    }

    if (current_field->shallow.is_above(bottom_right)) {
      add_shallow_bump(top_left, current_field);
      return check_field(current_field, active_fields);
    }

    if (current_field->steep.is_below(top_left)) {
      add_steep_bump(bottom_right, current_field);
      return check_field(current_field, active_fields);
    }

    auto steeper = current_field;
    auto shallower = active_fields.insert(current_field, *current_field);
    add_steep_bump(bottom_right, shallower);
    check_field(shallower, active_fields);
    add_shallow_bump(top_left, steeper);
    return check_field(steeper, active_fields);
  }

  void PermissiveVisibility::add_shallow_bump(Vec2I position, FieldIterator current_field)
  {
    current_field->shallow.far = position;
    current_field->shallow_bump.push_front(position);

    for (auto& bump : current_field->steep_bump) {
      if (current_field->shallow.is_above(bump)) {
        current_field->shallow.near = bump;
      }
    }
  }

  void PermissiveVisibility::add_steep_bump(Vec2I position, FieldIterator current_field)
  {
    current_field->steep.far = position;
    current_field->steep_bump.push_front(position);

    for (auto& bump : current_field->shallow_bump) {
      if (current_field->steep.is_below(bump)) {
        current_field->steep.near = bump;
      }
    }
  }

  auto PermissiveVisibility::check_field(FieldIterator current_field, FieldList& active_fields) -> FieldIterator
  {
    if (current_field->shallow.contains(current_field->steep.near) && current_field->shallow.contains(current_field->steep.far) && (current_field->shallow.contains({ 0, 1 }) || current_field->shallow.contains({ 1, 0 }))) {
      return active_fields.erase(current_field);
    }

    return current_field;
  }

  /*
   * ImprovedVisibility
   */

  ImprovedVisibility::ImprovedVisibility(GridMap* map, Flags<CellProperty> properties)
  : m_map(map)
  , m_properties(properties)
  {
  }

  void ImprovedVisibility::compute_visibility(Vec2I origin, int range_limit) const
  {
    if (m_map == nullptr) {
      return;
    }

    m_map->add_properties(origin, m_properties);

    for (int octant = 0; octant < 8; ++octant) {
      compute(octant, origin, range_limit, 1, { 1, 1 }, { 0, 1 });
    }
  }

  // NOLINTNEXTLINE(misc-no-recursion,readability-function-cognitive-complexity)
  void ImprovedVisibility::compute(int octant, Vec2I origin, int range_limit, int x, details::Slope top, details::Slope bottom) const
  {
    const int x_limit = range_limit >= 0 ? range_limit : std::numeric_limits<int>::max();
    const int square_range_limit = square(range_limit);

    for (; x <= x_limit; ++x) {
      int top_y = 0;
      if (top.x == 1) {
        top_y = x;
      } else {
        top_y = ((x * 2 - 1) * top.y + top.x) / (top.x * 2);

        if (!transparent(octant, origin, { x, top_y })) {
          if (top >= details::slope(top_y * 2 + 1, x * 2) && transparent(octant, origin, { x, top_y + 1 })) {
            ++top_y;
          }
        } else {
          int ax = x * 2;

          if (!transparent(octant, origin, { x + 1, top_y + 1 })) {
            ++ax;
          }

          if (top > details::slope(top_y * 2 + 1, ax)) {
            ++top_y;
          }
        }
      }

      int bottom_y = 0;

      if (bottom.y == 0) {
        bottom_y = 0;
      } else {
        bottom_y = ((x * 2 - 1) * bottom.y + bottom.x) / (bottom.x * 2);

        if (bottom >= details::slope(bottom_y * 2 + 1, x * 2) && !transparent(octant, origin, { x, bottom_y }) && transparent(octant, origin, { x, bottom_y + 1 })) {
          ++bottom_y;
        }
      }

      Opacity was_opaque = Opacity::Unknown;

      for (int y = top_y; y >= bottom_y; --y) {
        Vec2I local = { x, y };

        if (range_limit < 0 || square_length(local) <= square_range_limit) {
          bool is_opaque = !transparent(octant, origin, local);
          bool is_visible = is_opaque || ((y != top_y || top > details::slope(y * 4 - 1, x * 4 + 1)) && (y != bottom_y || bottom < details::slope(y * 4 + 1, x * 4 - 1)));

          if (is_visible) {
            set_visible(octant, origin, local);
          }

          if (x != x_limit) {

            if (is_opaque) {
              if (was_opaque == Opacity::Transparent) {
                details::Slope new_bottom = { y * 2 + 1, x * 2 };

                if (!transparent(octant, origin, { x, y + 1 })) {
                  --new_bottom.x;
                }

                if (top > new_bottom) {
                  if (y == bottom_y) {
                    bottom = new_bottom;
                    break;
                  }

                  compute(octant, origin, range_limit, x + 1, top, new_bottom);
                } else {
                  if (y == bottom_y) {
                    return;
                  }
                }
              }

              was_opaque = Opacity::Opaque;
            } else {
              if (was_opaque == Opacity::Opaque) {
                details::Slope new_top = { y * 2 + 1, x * 2 };

                if (!transparent(octant, origin, { x + 1, y + 1 })) {
                  ++new_top.x;
                }

                if (bottom >= new_top) {
                  return;
                }

                top = new_top;
              }

              was_opaque = Opacity::Transparent;
            }
          }
        }
      }

      if (was_opaque != Opacity::Transparent) {
        break;
      }
    }
  }

  bool ImprovedVisibility::transparent(int octant, Vec2I origin, Vec2I local) const
  {
    return m_map->transparent(transform_octant(origin, local, octant));
  }

  void ImprovedVisibility::set_visible(int octant, Vec2I origin, Vec2I local) const
  {
    m_map->add_properties(transform_octant(origin, local, octant), m_properties);
  }

}
