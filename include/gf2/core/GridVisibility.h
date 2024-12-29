// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_GRID_VISIBILITY_H
#define GF_GRID_VISIBILITY_H

#include <cstdint>

#include <forward_list>
#include <list>

#include "CoreApi.h"
#include "GridTypes.h"
#include "Vec2.h"

namespace gf {
  class GridMap;

  namespace details {
    struct Slope { // slope y/x as a rational number
      int32_t y;
      int32_t x;
    };

    constexpr Slope slope(int32_t y, int32_t x) { return { y, x }; }

    constexpr bool operator<(Slope lhs, Slope rhs) { return lhs.y * rhs.x < lhs.x * rhs.y; }
    constexpr bool operator<=(Slope lhs, Slope rhs) { return lhs.y * rhs.x <= lhs.x * rhs.y; }
    constexpr bool operator>(Slope lhs, Slope rhs) { return lhs.y * rhs.x > lhs.x * rhs.y; }
    constexpr bool operator>=(Slope lhs, Slope rhs) { return lhs.y * rhs.x >= lhs.x * rhs.y; }

  }

  class GF_CORE_API RayCastVisibility {
  public:
    RayCastVisibility(GridMap* map, Flags<CellProperty> properties);
    void compute_visibility(Vec2I origin, int range_limit) const;

  private:
    void trace_line(Vec2I origin, Vec2I target, int range_limit) const;

    GridMap* m_map;
    Flags<CellProperty> m_properties;
  };

  class GF_CORE_API ShadowCastVisibility {
  public:
    ShadowCastVisibility(GridMap* map, Flags<CellProperty> properties);
    void compute_visibility(Vec2I origin, int range_limit) const;

  private:
    void compute(int octant, Vec2I origin, int range_limit, int x, details::Slope top, details::Slope bottom) const;

    GridMap* m_map;
    Flags<CellProperty> m_properties;
  };

  class GF_CORE_API DiamondWallsVisibility {
  public:
    DiamondWallsVisibility(GridMap* map, Flags<CellProperty> properties);
    void compute_visibility(Vec2I origin, int range_limit) const;

  private:
    void compute(int octant, Vec2I origin, int range_limit, int x, details::Slope top, details::Slope bottom) const;
    bool transparent(int octant, Vec2I origin, Vec2I local) const;

    GridMap* m_map;
    Flags<CellProperty> m_properties;
  };

  class GF_CORE_API PermissiveVisibility {
  public:
    PermissiveVisibility(GridMap* map, Flags<CellProperty> properties);
    void compute_visibility(Vec2I origin, int range_limit) const;

  private:
    struct Line {
      Line(Vec2I near, Vec2I far)
      : near(near), far(far)
      {
      }

      Vec2I near = { 0, 0 };
      Vec2I far = { 0, 0 };

      bool is_below(Vec2I point) const { return relative_slope(point) > 0; }
      bool is_below_or_contains(Vec2I point) const { return relative_slope(point) >= 0; }
      bool is_above(Vec2I point) const { return relative_slope(point) < 0; }
      bool is_above_or_contains(Vec2I point) const { return relative_slope(point) <= 0; }
      bool contains(Vec2I point) const { return relative_slope(point) == 0; }

      int relative_slope(Vec2I point) const
      {
        return ((far.y - near.y) * (far.x - point.x)) - ((far.y - point.y) * (far.x - near.x));
      }
    };

    struct Field {
      Field(Line steep, Line shallow)
      : steep(steep), shallow(shallow)
      {
      }

      std::forward_list<Vec2I> steep_bump;
      std::forward_list<Vec2I> shallow_bump;
      Line steep;
      Line shallow;
    };

    using FieldList = std::list<Field>;
    using FieldIterator = FieldList::iterator;

    void compute_quadrant(Vec2I quadrant, Vec2I origin, int range_limit) const;
    bool blocked(Vec2I local, Vec2I quadrant, Vec2I origin, int range_limit) const;
    FieldIterator visit_square(Vec2I local, Vec2I quadrant, Vec2I origin, int range_limit, FieldIterator current_field, FieldList& active_fields) const;

    static void add_shallow_bump(Vec2I position, FieldIterator current_field);
    static void add_steep_bump(Vec2I position, FieldIterator current_field);
    static FieldIterator check_field(FieldIterator current_field, FieldList& active_fields);

    GridMap* m_map;
    Flags<CellProperty> m_properties;
  };

  class GF_CORE_API ImprovedVisibility {
  public:
    ImprovedVisibility(GridMap* map, Flags<CellProperty> properties);
    void compute_visibility(Vec2I origin, int range_limit) const;

  private:
    void compute(int octant, Vec2I origin, int range_limit, int x, details::Slope top, details::Slope bottom) const;
    bool transparent(int octant, Vec2I origin, Vec2I local) const;
    void set_visible(int octant, Vec2I origin, Vec2I local) const;

    GridMap* m_map;
    Flags<CellProperty> m_properties;
  };

}

#endif // GF_GRID_VISIBILITY_H
