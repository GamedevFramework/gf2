// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/GridMap.h>
// clang-format on

#include <algorithm>

#include <gf2/BinaryHeap.h>

namespace gf {
  using namespace operators;

  GridMap::GridMap(Vec2I size, AnyGrid grid)
  : m_cells(size, CellProperty::Transparent | CellProperty::Walkable)
  , m_grid(grid)
  {
  }

  GridMap GridMap::make_orthogonal(Vec2I size)
  {
    return { size, AnyGrid::make_orthogonal(size, vec(1.0f, 1.0f)) };
  }

  GridMap GridMap::make_isometric(Vec2I size)
  {
    return { size, AnyGrid::make_isometric(size, vec(1.0f, 1.0f)) };
  }

  GridMap GridMap::make_staggered(Vec2I size, CellAxis axis, CellIndex index)
  {
    return { size, AnyGrid::make_staggered(size, vec(1.0f, 1.0f), axis, index) };
  }

  GridMap GridMap::make_hexagonal(Vec2I size, CellAxis axis, CellIndex index)
  {
    return { size, AnyGrid::make_hexagonal(size, 1.0f, axis, index) };
  }

  const AnyGrid& GridMap::grid() const
  {
    return m_grid;
  }

  Vec2I GridMap::size() const
  {
    return m_cells.size();
  }

  PositionRange GridMap::position_range() const
  {
    return m_cells.position_range();
  }

  void GridMap::reset(Flags<CellProperty> properties)
  {
    for (auto& cell : m_cells) {
      cell = properties;
    }
  }

  void GridMap::set_properties(Vec2I position, Flags<CellProperty> properties)
  {
    m_cells(position) = properties;
  }

  bool GridMap::transparent(Vec2I position) const
  {
    return m_cells(position).test(CellProperty::Transparent);
  }

  void GridMap::set_transparent(Vec2I position, bool transparent)
  {
    if (transparent) {
      m_cells(position).set(CellProperty::Transparent);
    } else {
      m_cells(position).reset(CellProperty::Transparent);
    }
  }

  bool GridMap::walkable(Vec2I position) const
  {
    return m_cells(position).test(CellProperty::Walkable);
  }

  void GridMap::set_walkable(Vec2I position, bool walkable)
  {
    if (walkable) {
      m_cells(position).set(CellProperty::Walkable);
    } else {
      m_cells(position).reset(CellProperty::Walkable);
    }
  }

  void GridMap::set_empty(Vec2I position)
  {
    m_cells(position) |= (CellProperty::Transparent | CellProperty::Walkable);
  }

  /*
   * field of vision
   */

  void GridMap::clear_visible()
  {
    for (auto& cell : m_cells) {
      cell.reset(CellProperty::Visible);
    }
  }

  void GridMap::clear_explored()
  {
    for (auto& cell : m_cells) {
      cell.reset(CellProperty::Explored);
    }
  }

  bool GridMap::visible(Vec2I position) const
  {
    return m_cells(position).test(CellProperty::Visible);
  }

  bool GridMap::explored(Vec2I position) const
  {
    return m_cells(position).test(CellProperty::Explored);
  }

  /*
   * route
   */

  namespace {

    // Dijkstra

    struct DijkstraHeapData {
      Vec2I position = {};
      float distance = 0.0f;
    };

    bool operator<(const DijkstraHeapData& lhs, const DijkstraHeapData& rhs)
    {
      return lhs.distance > rhs.distance;
    }

    using DijkstraHeap = BinaryHeap<DijkstraHeapData>;

    struct DijkstraResultData {
      float distance = 0.0f;
      Vec2I previous = {};
      DijkstraHeap::handle_type handle;
    };

    std::vector<Vec2I> compute_dijkstra(const Array2D<Flags<CellProperty>>& cells, const AnyGrid& grid, Vec2I origin, Vec2I target, float diagonal_cost)
    {
      const Vec2I size = cells.size();
      Flags<CellNeighborQuery> flags = CellNeighborQuery::Valid;

      if (diagonal_cost > 0) {
        flags |= CellNeighborQuery::Diagonal;
      }

      DijkstraResultData default_result;
      default_result.distance = std::numeric_limits<float>::infinity();
      default_result.previous = vec(-1, -1);

      Array2D<DijkstraResultData> results(size, default_result);
      results(origin).distance = 0.0f;

      DijkstraHeap heap;

      for (auto position : cells.position_range()) {
        const auto cell = cells(position);

        if (!cell.test(CellProperty::Walkable)) {
          continue;
        }

        DijkstraHeapData data = {};
        data.position = position;
        data.distance = results(position).distance;

        results(position).handle = heap.push(data);
      }

      while (!heap.empty()) {
        const DijkstraHeapData data = heap.top();
        heap.pop();

        auto neighbors = grid.compute_neighbors(data.position, flags);

        for (auto position : neighbors) {
          assert(position != data.position);

          const Flags<CellProperty>& value = cells(position);

          if (!value.test(CellProperty::Walkable)) {
            continue;
          }

          const bool is_diagonal = grid.are_diagonal_neighbors(data.position, position);
          assert(diagonal_cost > 0 || !is_diagonal);

          const float updated_distance = results(data.position).distance + (is_diagonal ? diagonal_cost : 1.0f);

          if (updated_distance < results(position).distance) {
            auto& result = results(position);
            result.distance = updated_distance;
            result.previous = data.position;

            assert(heap(result.handle).position == position);
            heap(result.handle).distance = updated_distance;
            heap.increase(result.handle);
          }
        }
      }

      std::vector<Vec2I> route;
      Vec2I current = target;

      while (current != origin) {
        if (current.x == -1 || current.y == -1) {
          return {};
        }

        route.push_back(current);
        current = results(current).previous;
      }

      route.push_back(origin);
      std::reverse(route.begin(), route.end());

      assert(!route.empty());

      return route;
    }

  }

  std::vector<Vec2I> GridMap::compute_route(Vec2I origin, Vec2I target, float diagonal_cost, Route algorithm)
  {
    switch (algorithm) {
      case Route::Dijkstra:
        return compute_dijkstra(m_cells, m_grid, origin, target, diagonal_cost);
      default:
        break;
    }

    return {};
  }

}
