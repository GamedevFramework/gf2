// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_ANY_GRID_H
#define GF_ANY_GRID_H

#include <variant>
#include <vector>

#include "CoreApi.h"
#include "Grids.h"

namespace gf {

  class GF_CORE_API AnyGrid {
  public:
    AnyGrid() = default;

    static AnyGrid make_orthogonal(Vec2I layer_size, Vec2I tile_size);
    static AnyGrid make_isometric(Vec2I layer_size, Vec2I tile_size);
    static AnyGrid make_staggered(Vec2I layer_size, Vec2I tile_size, CellAxis axis, CellIndex index);
    static AnyGrid make_hexagonal(Vec2I layer_size, Vec2I tile_size, int32_t side_length, CellAxis axis, CellIndex index);
    static AnyGrid make_hexagonal(Vec2I layer_size, float radius, CellAxis axis, CellIndex index);

    RectI compute_bounds() const;
    RectI compute_visible_area(RectF local) const;
    RectI compute_cell_bounds(Vec2I position) const;
    Vec2I compute_position(Vec2F location) const;
    std::vector<Vec2I> compute_contour(Vec2I position) const;
    std::vector<Vec2I> compute_neighbors(Vec2I position, Flags<CellNeighborQuery> flags = None) const;
    bool are_diagonal_neighbors(Vec2I position0, Vec2I position1) const;

    GridOrientation orientation() const;

  private:
    AnyGrid(OrthogonalGrid grid);
    AnyGrid(IsometricGrid grid);
    AnyGrid(StaggeredGrid grid);
    AnyGrid(HexagonalGrid grid);

    std::variant<std::monostate, OrthogonalGrid, IsometricGrid, StaggeredGrid, HexagonalGrid> m_variant;
  };

}

#endif // GF_ANY_GRID_H
