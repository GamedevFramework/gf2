// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Grid.h>
// clang-format on

namespace gf {

  Grid::Grid(OrthogonalGrid grid)
  : m_variant(grid)
  {
  }

  Grid::Grid(StaggeredGrid grid)
  : m_variant(grid)
  {
  }

  Grid::Grid(HexagonalGrid grid)
  : m_variant(grid)
  {
  }

  Grid Grid::make_orthogonal(Vec2F tile_size)
  {
    return { OrthogonalGrid(tile_size) };
  }

  Grid Grid::make_staggered(Vec2F tile_size, CellAxis axis, CellIndex index)
  {
    return { StaggeredGrid(tile_size, axis, index) };
  }

  Grid Grid::make_hexagonal(Vec2F tile_size, float side_length, CellAxis axis, CellIndex index)
  {
    return { HexagonalGrid(tile_size, side_length, axis, index) };
  }

  Grid Grid::make_hexagonal(float radius, CellAxis axis, CellIndex index)
  {
    return { HexagonalGrid(radius, axis, index) };
  }

  RectF Grid::compute_bounds(Vec2I layer_size) const
  {
    return std::visit([=](auto&& grid) {
      using T = std::decay_t<decltype(grid)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        return RectF::from_size({ 0.0f, 0.0f });
      } else {
        return grid.compute_bounds(layer_size);
      }
    },
        m_variant);
  }

  RectI Grid::compute_visible_area(RectF local) const
  {
    return std::visit([=](auto&& grid) {
      using T = std::decay_t<decltype(grid)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        return RectI::from_size({ 0, 0 });
      } else {
        return grid.compute_visible_area(local);
      }
    },
        m_variant);
  }

  RectF Grid::compute_cell_bounds(Vec2I coordinates) const
  {
    return std::visit([=](auto&& grid) {
      using T = std::decay_t<decltype(grid)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        return RectF::from_size({ 0.0f, 0.0f });
      } else {
        return grid.compute_cell_bounds(coordinates);
      }
    },
        m_variant);
  }

  Vec2I Grid::compute_coordinates(Vec2F position) const
  {
    return std::visit([=](auto&& grid) {
      using T = std::decay_t<decltype(grid)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        return Vec2I(0, 0);
      } else {
        return grid.compute_coordinates(position);
      }
    },
        m_variant);
  }

  std::vector<Vec2F> Grid::compute_contour(Vec2I coordinates) const
  {
    return std::visit([=](auto&& grid) {
      using T = std::decay_t<decltype(grid)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        std::vector<Vec2F> empty;
        return empty;
      } else {
        return grid.compute_contour(coordinates);
      }
    },
        m_variant);
  }

  std::vector<Vec2I> Grid::compute_neighbors(Vec2I coordinates, Vec2I layer_size, Flags<CellNeighborQuery> flags) const
  {
    return std::visit([=](auto&& grid) {
      using T = std::decay_t<decltype(grid)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        std::vector<Vec2I> empty;
        return empty;
      } else {
        return grid.compute_neighbors(coordinates, layer_size, flags);
      }
    },
        m_variant);
  }

} // namespace gf
