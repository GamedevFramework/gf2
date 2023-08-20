// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/AnyGrid.h>
// clang-format on

namespace gf {

  AnyGrid::AnyGrid(OrthogonalGrid grid)
  : m_variant(grid)
  {
  }

  AnyGrid::AnyGrid(IsometricGrid grid)
  : m_variant(grid)
  {
  }

  AnyGrid::AnyGrid(StaggeredGrid grid)
  : m_variant(grid)
  {
  }

  AnyGrid::AnyGrid(HexagonalGrid grid)
  : m_variant(grid)
  {
  }

  AnyGrid AnyGrid::make_orthogonal(Vec2I layer_size, Vec2F tile_size)
  {
    return { OrthogonalGrid(layer_size, tile_size) };
  }

  AnyGrid AnyGrid::make_isometric(Vec2I layer_size, Vec2F tile_size)
  {
    return { IsometricGrid(layer_size, tile_size) };
  }

  AnyGrid AnyGrid::make_staggered(Vec2I layer_size, Vec2F tile_size, CellAxis axis, CellIndex index)
  {
    return { StaggeredGrid(layer_size, tile_size, axis, index) };
  }

  AnyGrid AnyGrid::make_hexagonal(Vec2I layer_size, Vec2F tile_size, float side_length, CellAxis axis, CellIndex index)
  {
    return { HexagonalGrid(layer_size, tile_size, side_length, axis, index) };
  }

  AnyGrid AnyGrid::make_hexagonal(Vec2I layer_size, float radius, CellAxis axis, CellIndex index)
  {
    return { HexagonalGrid(layer_size, radius, axis, index) };
  }

  RectF AnyGrid::compute_bounds() const
  {
    return std::visit([=](auto&& grid) {
      using T = std::decay_t<decltype(grid)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        return RectF::from_size({ 0.0f, 0.0f });
      } else {
        return grid.compute_bounds();
      }
    },
        m_variant);
  }

  RectI AnyGrid::compute_visible_area(RectF local) const
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

  RectF AnyGrid::compute_cell_bounds(Vec2I coordinates) const
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

  Vec2I AnyGrid::compute_coordinates(Vec2F position) const
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

  std::vector<Vec2F> AnyGrid::compute_contour(Vec2I coordinates) const
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

  std::vector<Vec2I> AnyGrid::compute_neighbors(Vec2I coordinates, Flags<CellNeighborQuery> flags) const
  {
    return std::visit([=](auto&& grid) {
      using T = std::decay_t<decltype(grid)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        std::vector<Vec2I> empty;
        return empty;
      } else {
        return grid.compute_neighbors(coordinates, flags);
      }
    },
        m_variant);
  }

  bool AnyGrid::are_diagonal_neighbors(Vec2I coordinates0, Vec2I coordinates1) const
  {
    return std::visit([=](auto&& grid) {
      using T = std::decay_t<decltype(grid)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        return false;
      } else {
        return grid.are_diagonal_neighbors(coordinates0, coordinates1);
      }
    },
        m_variant);
  }

} // namespace gf