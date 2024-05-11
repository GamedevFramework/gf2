// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_GRID_VISIBILITY_H
#define GF_GRID_VISIBILITY_H

#include <cstdint>

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
