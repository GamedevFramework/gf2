// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_GRIDS_H
#define GF_GRIDS_H

#include <vector>

#include "Flags.h"
#include "GridTypes.h"
#include "Rect.h"
#include "Vec2.h"

namespace gf {

  class GF_CORE_API OrthogonalGrid {
  public:
    OrthogonalGrid(Vec2I layer_size, Vec2I tile_size);

    RectI compute_bounds() const;
    RectI compute_visible_area(RectF local) const;
    RectI compute_cell_bounds(Vec2I position) const;
    Vec2I compute_position(Vec2F location) const;
    std::vector<Vec2I> compute_contour(Vec2I position) const;
    std::vector<Vec2I> compute_neighbors(Vec2I position, Flags<CellNeighborQuery> flags = None) const;
    bool are_diagonal_neighbors(Vec2I position0, Vec2I position1) const;

  private:
    Vec2I m_layer_size;
    Vec2I m_tile_size;
  };

  class GF_CORE_API IsometricGrid {
  public:
    IsometricGrid(Vec2I layer_size, Vec2I tile_size);

    RectI compute_bounds() const;
    RectI compute_visible_area(RectF local) const;
    RectI compute_cell_bounds(Vec2I position) const;
    Vec2I compute_position(Vec2F location) const;
    std::vector<Vec2I> compute_contour(Vec2I position) const;
    std::vector<Vec2I> compute_neighbors(Vec2I position, Flags<CellNeighborQuery> flags = None) const;
    bool are_diagonal_neighbors(Vec2I position0, Vec2I position1) const;

  private:
    Vec2I m_layer_size;
    Vec2I m_tile_size;
  };

  class GF_CORE_API StaggeredGrid {
  public:
    StaggeredGrid(Vec2I layer_size, Vec2I tile_size, CellAxis axis, CellIndex index);

    RectI compute_bounds() const;
    RectI compute_visible_area(RectF local) const;
    RectI compute_cell_bounds(Vec2I position) const;
    Vec2I compute_position(Vec2F location) const;
    std::vector<Vec2I> compute_contour(Vec2I position) const;
    std::vector<Vec2I> compute_neighbors(Vec2I position, Flags<CellNeighborQuery> flags = None) const;
    bool are_diagonal_neighbors(Vec2I position0, Vec2I position1) const;

  private:
    Vec2I compute_position_x_axis(bool is_diagonally_split, Vec2I q, Vec2F r) const;
    Vec2I compute_position_y_axis(bool is_diagonally_split, Vec2I q, Vec2F r) const;

    Vec2I m_layer_size;
    Vec2I m_tile_size;
    CellAxis m_axis;
    CellIndex m_index;
  };

  class GF_CORE_API HexagonalGrid {
  public:
    HexagonalGrid(Vec2I layer_size, Vec2I tile_size, int32_t side_length, CellAxis axis, CellIndex index);
    HexagonalGrid(Vec2I layer_size, float radius, CellAxis axis, CellIndex index);

    RectI compute_bounds() const;
    RectI compute_visible_area(RectF local) const;
    RectI compute_cell_bounds(Vec2I position) const;
    Vec2I compute_position(Vec2F location) const;
    std::vector<Vec2I> compute_contour(Vec2I position) const;
    std::vector<Vec2I> compute_neighbors(Vec2I position, Flags<CellNeighborQuery> flags = None) const;
    bool are_diagonal_neighbors(Vec2I position0, Vec2I position1) const;

    static Vec2I compute_regular_size(CellAxis axis, float radius);

  private:
    Vec2I compute_position_x_axis(Vec2F location) const;
    Vec2I compute_position_y_axis(Vec2F location) const;

    Vec2I m_layer_size;
    Vec2I m_tile_size;
    int32_t m_side_length;
    CellAxis m_axis;
    CellIndex m_index;
  };

} // namespace gf

#endif // GF_GRIDS_H
