// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_GRID_MAP_H
#define GF_GRID_MAP_H

#include <vector>

#include "AnyGrid.h"
#include "Array2D.h"
#include "CoreApi.h"
#include "Flags.h"
#include "Math.h"

namespace gf {

  enum class CellProperty : uint8_t {
    Transparent = 0x01,
    Walkable = 0x02,
    Visible = 0x10,
    Explored = 0x20,
  };

  template<>
  struct EnableBitmaskOperators<CellProperty> : std::true_type {
  };

  enum class Route : uint8_t {
    AStar,
    Dijkstra,
  };

  class GF_CORE_API GridMap {
  public:
    GridMap() = default;

    static GridMap make_orthogonal(Vec2I size);
    static GridMap make_isometric(Vec2I size);
    static GridMap make_staggered(Vec2I size, CellAxis axis, CellIndex index);
    static GridMap make_hexagonal(Vec2I size, CellAxis axis, CellIndex index);

    const AnyGrid& grid() const;

    Vec2I size() const;
    PositionRange position_range() const;

    void reset(Flags<CellProperty> properties);

    void set_properties(Vec2I position, Flags<CellProperty> properties);

    bool transparent(Vec2I position) const;
    void set_transparent(Vec2I position, bool transparent = true);

    bool walkable(Vec2I position) const;
    void set_walkable(Vec2I position, bool walkable = true);

    void set_empty(Vec2I position);

    void clear_visible();
    void clear_explored();

    bool visible(Vec2I position) const;
    bool explored(Vec2I position) const;

    std::vector<Vec2I> compute_route(Vec2I origin, Vec2I target, float diagonal_cost = Sqrt2, Route route = Route::AStar);

  private:
    GridMap(Vec2I size, AnyGrid grid);

    Array2D<Flags<CellProperty>> m_cells;
    AnyGrid m_grid;
  };

}

#endif // GF_GRID_MAP_H
