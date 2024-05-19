// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/GridMap.h>

#include <algorithm>

#include <gf2/core/BinaryHeap.h>
#include "gf2/core/GridTypes.h"
#include <gf2/core/Log.h>
#include <gf2/core/GridVisibility.h>

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

  GridMap GridMap::make_orthogonal(Vec2I layer_size, Vec2I tile_size)
  {
    return { layer_size, AnyGrid::make_orthogonal(layer_size, tile_size) };
  }

  GridMap GridMap::make_isometric(Vec2I size)
  {
    return { size, AnyGrid::make_isometric(size, vec(1.0f, 1.0f)) };
  }

  GridMap GridMap::make_isometric(Vec2I layer_size, Vec2I tile_size)
  {
    return { layer_size, AnyGrid::make_isometric(layer_size, tile_size) };
  }

  GridMap GridMap::make_staggered(Vec2I size, CellAxis axis, CellIndex index)
  {
    return { size, AnyGrid::make_staggered(size, vec(1.0f, 1.0f), axis, index) };
  }

  GridMap GridMap::make_staggered(Vec2I layer_size, Vec2I tile_size, CellAxis axis, CellIndex index)
  {
    return { layer_size, AnyGrid::make_staggered(layer_size, tile_size, axis, index) };
  }

  GridMap GridMap::make_hexagonal(Vec2I size, CellAxis axis, CellIndex index)
  {
    return { size, AnyGrid::make_hexagonal(size, 1.0f, axis, index) };
  }

  GridMap GridMap::make_hexagonal(Vec2I layer_size, Vec2I tile_size, int32_t side_length, CellAxis axis, CellIndex index)
  {
    return { layer_size, AnyGrid::make_hexagonal(layer_size, tile_size, side_length, axis, index) };
  }

  GridMap GridMap::make_hexagonal(Vec2I layer_size, float radius, CellAxis axis, CellIndex index)
  {
    return { layer_size, AnyGrid::make_hexagonal(layer_size, radius, axis, index) };
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
    if (!m_cells.valid(position)) {
      return;
    }

    m_cells(position) = properties;
  }

  void GridMap::add_properties(Vec2I position, Flags<CellProperty> properties)
  {
    if (!m_cells.valid(position)) {
      return;
    }

    m_cells(position) |= properties;
  }

  bool GridMap::transparent(Vec2I position) const
  {
    if (!m_cells.valid(position)) {
      return false;
    }

    return m_cells(position).test(CellProperty::Transparent);
  }

  void GridMap::set_transparent(Vec2I position, bool transparent)
  {
    if (!m_cells.valid(position)) {
      return;
    }

    if (transparent) {
      m_cells(position).set(CellProperty::Transparent);
    } else {
      m_cells(position).reset(CellProperty::Transparent);
    }
  }

  bool GridMap::walkable(Vec2I position) const
  {
    if (!m_cells.valid(position)) {
      return false;
    }

    return m_cells(position).test(CellProperty::Walkable);
  }

  void GridMap::set_walkable(Vec2I position, bool walkable)
  {
    if (!m_cells.valid(position)) {
      return;
    }

    if (walkable) {
      m_cells(position).set(CellProperty::Walkable);
    } else {
      m_cells(position).reset(CellProperty::Walkable);
    }
  }

  void GridMap::set_empty(Vec2I position)
  {
    if (!m_cells.valid(position)) {
      return;
    }

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
    if (!m_cells.valid(position)) {
      return false;
    }

    return m_cells(position).test(CellProperty::Visible);
  }

  bool GridMap::explored(Vec2I position) const
  {
    if (!m_cells.valid(position)) {
      return false;
    }

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

    struct DijkstraCellData {
      float distance = std::numeric_limits<float>::infinity();
      Vec2I previous = vec(-1, -1);
      DijkstraHeap::handle_type handle = {};
    };

    using DijkstraData = Array2D<DijkstraCellData>;

    class DijkstraAlgorithm {
    public:
      DijkstraAlgorithm(const Array2D<Flags<CellProperty>>& cells, const AnyGrid& grid)
      : m_cells(cells)
      , m_grid(grid)
      , m_data(cells.size())
      {
      }

      std::vector<Vec2I> operator()(Vec2I origin, Vec2I target, float diagonal_cost)
      {
        Flags<CellNeighborQuery> flags = CellNeighborQuery::Valid;

        if (diagonal_cost > 0) {
          flags |= CellNeighborQuery::Diagonal;
        }

        initialize(origin);

        while (!m_heap.empty()) {
          const DijkstraHeapData data = m_heap.top();
          m_heap.pop();
          compute_node(data, diagonal_cost, flags);
        }

        return compute_route(origin, target);
      }

    private:
      void initialize(Vec2I origin)
      {
        m_data(origin).distance = 0.0f;

        for (auto position : m_cells.position_range()) {
          const auto cell = m_cells(position);

          if (!cell.test(CellProperty::Walkable)) {
            continue;
          }

          DijkstraHeapData data = {};
          data.position = position;
          data.distance = m_data(position).distance;

          m_data(position).handle = m_heap.push(data);
        }
      }

      void compute_node(DijkstraHeapData heap_data, float diagonal_cost, Flags<CellNeighborQuery> flags)
      {
        auto neighbors = m_grid.compute_neighbors(heap_data.position, flags);

        for (auto position : neighbors) {
          assert(position != heap_data.position);

          const Flags<CellProperty>& value = m_cells(position);

          if (!value.test(CellProperty::Walkable)) {
            continue;
          }

          const bool is_diagonal = m_grid.are_diagonal_neighbors(heap_data.position, position);
          assert(diagonal_cost > 0 || !is_diagonal);

          const float updated_distance = m_data(heap_data.position).distance + (is_diagonal ? diagonal_cost : 1.0f);

          if (updated_distance < m_data(position).distance) {
            auto& data = m_data(position);
            data.distance = updated_distance;
            data.previous = heap_data.position;

            assert(m_heap(data.handle).position == position);
            m_heap(data.handle).distance = updated_distance;
            m_heap.increase(data.handle);
          }
        }
      }

      std::vector<Vec2I> compute_route(Vec2I origin, Vec2I target) const
      {
        std::vector<Vec2I> route;
        Vec2I current = target;

        while (current != origin) {
          if (current.x == -1 || current.y == -1) {
            return {};
          }

          route.push_back(current);
          current = m_data(current).previous;
        }

        route.push_back(origin);
        std::reverse(route.begin(), route.end());

        assert(!route.empty());
        return route;
      }

      const Array2D<Flags<CellProperty>>& m_cells; // NOLINT
      const AnyGrid& m_grid;                       // NOLINT

      DijkstraData m_data;
      DijkstraHeap m_heap;
    };
  }

  std::vector<Vec2I> GridMap::compute_route(Vec2I origin, Vec2I target, float diagonal_cost, Route route)
  {
    switch (route) {
      case Route::Dijkstra:
        {
          DijkstraAlgorithm algorithm(m_cells, m_grid);
          return algorithm(origin, target, diagonal_cost);
        }

      default:
        break;
    }

    return {};
  }

  void GridMap::compute_field_of_vision(Vec2I origin, int range_limit, Visibility visibility)
  {
    raw_compute_field_of_vision(origin, range_limit, CellProperty::Visible | CellProperty::Explored, visibility);
  }

  void GridMap::compute_local_field_of_vision(Vec2I origin, int range_limit, Visibility visibility)
  {
    raw_compute_field_of_vision(origin, range_limit, CellProperty::Visible, visibility);
  }

  void GridMap::raw_compute_field_of_vision(Vec2I origin, int range_limit, Flags<CellProperty> properties, Visibility visibility)
  {
    const GridOrientation orientation = m_grid.orientation();

    if (orientation == GridOrientation::Hexagonal || orientation == GridOrientation::Unknown) {
      Log::fatal("Unsupported orientation for field of vision.");
    }

    switch (visibility) {
      case Visibility::RayCast:
        {
          RayCastVisibility visibility(this, properties);
          visibility.compute_visibility(origin, range_limit);
        }
        break;
      case Visibility::ShadowCast:
        {
          ShadowCastVisibility visibility(this, properties);
          visibility.compute_visibility(origin, range_limit);
        }
        break;
      case Visibility::DiamondWalls:
        {
          DiamondWallsVisibility visibility(this, properties);
          visibility.compute_visibility(origin, range_limit);
        }
        break;
      case Visibility::Permissive:
        {
          PermissiveVisibility visibility(this, properties);
          visibility.compute_visibility(origin, range_limit);
        }
        break;
      case Visibility::Improved:
        {
          ImprovedVisibility visibility(this, properties);
          visibility.compute_visibility(origin, range_limit);
        }
        break;
    }
  }

}
