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
    OrthogonalGrid(Vec2F tile_size);

    RectF compute_bounds(Vec2I layer_size) const;
    RectI compute_visible_area(RectF local) const;
    RectF compute_cell_bounds(Vec2I coordinates) const;
    Vec2I compute_coordinates(Vec2F position) const;
    std::vector<Vec2F> compute_contour(Vec2I coordinates) const;
    std::vector<Vec2I> compute_neighbors(Vec2I coordinates, Vec2I layer_size, Flags<CellNeighborQuery> flags = None) const;
    bool are_diagonal_neighbors(Vec2I coordinates0, Vec2I coordinates1) const;

  private:
    Vec2F m_tile_size;
  };

  class GF_CORE_API StaggeredGrid {
  public:
    StaggeredGrid(Vec2F tile_size, CellAxis axis, CellIndex index);

    RectF compute_bounds(Vec2I layer_size) const;
    RectI compute_visible_area(RectF local) const;
    RectF compute_cell_bounds(Vec2I coordinates) const;
    Vec2I compute_coordinates(Vec2F position) const;
    std::vector<Vec2F> compute_contour(Vec2I coordinates) const;
    std::vector<Vec2I> compute_neighbors(Vec2I coordinates, Vec2I layer_size, Flags<CellNeighborQuery> flags = None) const;
    bool are_diagonal_neighbors(Vec2I coordinates0, Vec2I coordinates1) const;

  private:
    Vec2F m_tile_size;
    CellAxis m_axis;
    CellIndex m_index;
  };

  class GF_CORE_API HexagonalGrid {
  public:
    HexagonalGrid(Vec2F tile_size, float side_length, CellAxis axis, CellIndex index);
    HexagonalGrid(float radius, CellAxis axis, CellIndex index);

    RectF compute_bounds(Vec2I layer_size) const;
    RectI compute_visible_area(RectF local) const;
    RectF compute_cell_bounds(Vec2I coordinates) const;
    Vec2I compute_coordinates(Vec2F position) const;
    std::vector<Vec2F> compute_contour(Vec2I coordinates) const;
    std::vector<Vec2I> compute_neighbors(Vec2I coordinates, Vec2I layer_size, Flags<CellNeighborQuery> flags = None) const;
    bool are_diagonal_neighbors(Vec2I coordinates0, Vec2I coordinates1) const;

    static Vec2F compute_regular_size(CellAxis axis, float radius);

  private:
    Vec2F m_tile_size;
    float m_side_length;
    CellAxis m_axis;
    CellIndex m_index;
  };

} // namespace gf

#endif // GF_GRIDS_H
