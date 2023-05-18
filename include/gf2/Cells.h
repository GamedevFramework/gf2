// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CELLS_H
#define GF_CELLS_H

#include <variant>
#include <vector>

#include "CellTypes.h"
#include "Flags.h"
#include "Rect.h"
#include "Vec2.h"

namespace gf {

  enum class CellNeighborQuery : uint8_t {
    Valid = 0x01,
    Diagonal = 0x02,
  };

  class GF_CORE_API OrthogonalCells {
  public:
    OrthogonalCells(Vec2F tile_size);

    RectF compute_bounds(Vec2I layer_size) const;
    RectI compute_visible_area(RectF local) const;
    RectF compute_cell_bounds(Vec2I coordinates) const;
    Vec2I compute_coordinates(Vec2F position) const;
    std::vector<Vec2F> compute_contour(Vec2I coordinates) const;
    std::vector<Vec2I> compute_neighbors(Vec2I coordinates, Vec2I layer_size, Flags<CellNeighborQuery> flags = None) const;

  private:
    Vec2F m_tile_size;
  };

  class GF_CORE_API StaggeredCells {
  public:
    StaggeredCells(Vec2F tile_size, CellAxis axis, CellIndex index);

    RectF compute_bounds(Vec2I layer_size) const;
    RectI compute_visible_area(RectF local) const;
    RectF compute_cell_bounds(Vec2I coordinates) const;
    Vec2I compute_coordinates(Vec2F position) const;
    std::vector<Vec2F> compute_contour(Vec2I coordinates) const;
    std::vector<Vec2I> compute_neighbors(Vec2I coordinates, Vec2I layer_size, Flags<CellNeighborQuery> flags = None) const;

  private:
    Vec2F m_tile_size;
    CellAxis m_axis;
    CellIndex m_index;
  };

  class GF_CORE_API HexagonalCells {
  public:
    HexagonalCells(Vec2F tile_size, float side_length, CellAxis axis, CellIndex index);
    HexagonalCells(float radius, CellAxis axis, CellIndex index);

    RectF compute_bounds(Vec2I layer_size) const;
    RectI compute_visible_area(RectF local) const;
    RectF compute_cell_bounds(Vec2I coordinates) const;
    Vec2I compute_coordinates(Vec2F position) const;
    std::vector<Vec2F> compute_contour(Vec2I coordinates) const;
    std::vector<Vec2I> compute_neighbors(Vec2I coordinates, Vec2I layer_size, Flags<CellNeighborQuery> flags = None) const;

    static Vec2F compute_regular_size(CellAxis axis, float radius);

  private:
    Vec2F m_tile_size;
    float m_side_length;
    CellAxis m_axis;
    CellIndex m_index;
  };

  class GF_CORE_API Cells {
  public:
    Cells() = default;

    static Cells make_orthogonal(Vec2F tile_size);
    static Cells make_staggered(Vec2F tile_size, CellAxis axis, CellIndex index);
    static Cells make_hexagonal(Vec2F tile_size, float side_length, CellAxis axis, CellIndex index);
    static Cells make_hexagonal(float radius, CellAxis axis, CellIndex index);

    RectF compute_bounds(Vec2I layer_size) const;
    RectI compute_visible_area(RectF local) const;
    RectF compute_cell_bounds(Vec2I coordinates) const;
    Vec2I compute_coordinates(Vec2F position) const;
    std::vector<Vec2F> compute_contour(Vec2I coordinates) const;
    std::vector<Vec2I> compute_neighbors(Vec2I coordinates, Vec2I layer_size, Flags<CellNeighborQuery> flags = None) const;

  private:
    Cells(OrthogonalCells cells);
    Cells(StaggeredCells cells);
    Cells(HexagonalCells cells);

    std::variant<std::monostate, OrthogonalCells, StaggeredCells, HexagonalCells> m_variant;
  };

} // namespace gf

#endif // GF_CELLS_H
