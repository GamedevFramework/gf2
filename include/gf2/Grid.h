// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_GRID_H
#define GF_GRID_H

#include <variant>
#include <vector>

#include "Grids.h"

namespace gf {

  class GF_CORE_API Grid {
  public:
    Grid() = default;

    static Grid make_orthogonal(Vec2I layer_size, Vec2F tile_size);
    static Grid make_isometric(Vec2I layer_size, Vec2F tile_size);
    static Grid make_staggered(Vec2I layer_size, Vec2F tile_size, CellAxis axis, CellIndex index);
    static Grid make_hexagonal(Vec2I layer_size, Vec2F tile_size, float side_length, CellAxis axis, CellIndex index);
    static Grid make_hexagonal(Vec2I layer_size, float radius, CellAxis axis, CellIndex index);

    RectF compute_bounds() const;
    RectI compute_visible_area(RectF local) const;
    RectF compute_cell_bounds(Vec2I coordinates) const;
    Vec2I compute_coordinates(Vec2F position) const;
    std::vector<Vec2F> compute_contour(Vec2I coordinates) const;
    std::vector<Vec2I> compute_neighbors(Vec2I coordinates, Flags<CellNeighborQuery> flags = None) const;
    bool are_diagonal_neighbors(Vec2I coordinates0, Vec2I coordinates1) const;

  private:
    Grid(OrthogonalGrid grid);
    Grid(IsometricGrid grid);
    Grid(StaggeredGrid grid);
    Grid(HexagonalGrid grid);

    std::variant<std::monostate, OrthogonalGrid, IsometricGrid, StaggeredGrid, HexagonalGrid> m_variant;
  };

}

#endif // GF_GRID_H
