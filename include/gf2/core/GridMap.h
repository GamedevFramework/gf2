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

  enum class Route : uint8_t {
    AStar,
    Dijkstra,
  };

  enum class Visibility : uint8_t {
    RayCast,
    ShadowCast,
    DiamondWalls,
    Permissive,
    Improved,
  };

  class GF_CORE_API GridMap {
  public:
    GridMap() = default;

    static GridMap make_orthogonal(Vec2I size);
    static GridMap make_orthogonal(Vec2I layer_size, Vec2I tile_size);

    static GridMap make_isometric(Vec2I size);
    static GridMap make_isometric(Vec2I layer_size, Vec2I tile_size);

    static GridMap make_staggered(Vec2I size, CellAxis axis, CellIndex index);
    static GridMap make_staggered(Vec2I layer_size, Vec2I tile_size, CellAxis axis, CellIndex index);

    static GridMap make_hexagonal(Vec2I size, CellAxis axis, CellIndex index);
    static GridMap make_hexagonal(Vec2I layer_size, Vec2I tile_size, int32_t side_length, CellAxis axis, CellIndex index);
    static GridMap make_hexagonal(Vec2I layer_size, float radius, CellAxis axis, CellIndex index);

    const AnyGrid& grid() const;

    Vec2I size() const;
    PositionRange position_range() const;

    void reset(Flags<CellProperty> properties);

    void set_properties(Vec2I position, Flags<CellProperty> properties);
    void add_properties(Vec2I position, Flags<CellProperty> properties);

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

    void compute_field_of_vision(Vec2I origin, int range_limit, Visibility visibility);
    void compute_local_field_of_vision(Vec2I origin, int range_limit, Visibility visibility);

  private:
    GridMap(Vec2I size, AnyGrid grid);

    void raw_compute_field_of_vision(Vec2I origin, int range_limit, Flags<CellProperty> properties, Visibility visibility);

    Array2D<Flags<CellProperty>> m_cells;
    AnyGrid m_grid;
  };

}

#endif // GF_GRID_MAP_H
