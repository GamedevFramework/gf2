// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PATH_FINDING_H
#define GF_PATH_FINDING_H

#include <algorithm>

#include "Array2D.h"
#include "BinaryHeap.h"
#include "CoreApi.h"
#include "GridTypes.h"
#include "Vec2.h"

namespace gf {

  /*
   * Dijkstra
   */

  namespace details {

    struct DijkstraHeapData {
      Vec2I position = {};
      float distance = 0.0f;
    };

    inline bool operator<(const DijkstraHeapData& lhs, const DijkstraHeapData& rhs)
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

  }

  class GF_CORE_API DijkstraAlgorithm {
  public:

    template<typename Cell, typename Grid, typename CostFunction>
    std::vector<Vec2I> operator()(const Array2D<Cell>& cells, const Grid& grid, Vec2I origin, Vec2I target, CostFunction cost_function, Flags<CellNeighborQuery> flags)
    {
      initialize(cells, origin);

      while (!m_heap.empty()) {
        const details::DijkstraHeapData data = m_heap.top();
        m_heap.pop();
        compute_node(cells, grid, data, cost_function, flags);
      }

      return compute_route(origin, target);
    }

  private:
    template<typename Cell>
    void initialize(const Array2D<Cell>& cells, Vec2I origin)
    {
      m_data = details::DijkstraData(cells.size());
      m_heap.clear();

      m_data(origin).distance = 0.0f;

      for (const Vec2I position : cells.position_range()) {
        const Cell& cell = cells(position);

        if (!cell.walkable()) {
          continue;
        }

        details::DijkstraHeapData data = {};
        data.position = position;
        data.distance = m_data(position).distance;

        m_data(position).handle = m_heap.push(data);
      }
    }

    template<typename Cell, typename Grid, typename CostFunction>
    void compute_node(const Array2D<Cell>& cells, const Grid& grid,  details::DijkstraHeapData heap_data, CostFunction cost_function, Flags<CellNeighborQuery> flags)
    {
      const std::vector<Vec2I> neighbors = grid.compute_neighbors(heap_data.position, flags);

      for (const Vec2I position : neighbors) {
        assert(position != heap_data.position);

        const Cell& cell = cells(position);

        if (!cell.walkable()) {
          continue;
        }

        const float updated_distance = m_data(heap_data.position).distance + cost_function(heap_data.position, position);

        if (updated_distance < m_data(position).distance) {
          details::DijkstraCellData& data = m_data(position);
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

    details::DijkstraData m_data;
    details::DijkstraHeap m_heap;
  };


  template<typename Cell, typename Grid, typename CostFunction>
  std::vector<Vec2I> compute_route_dijkstra(const Array2D<Cell>& cells, const Grid& grid, Vec2I origin, Vec2I target, CostFunction cost_function, Flags<CellNeighborQuery> flags)
  {
    DijkstraAlgorithm algorithm = {};
    return algorithm(cells, grid, origin, target, cost_function, flags);
  }

  /*
   * AStar
   */

  namespace details {

    struct AStarHeapData {
      Vec2I position = {};
      float priority = 0.0f;
    };

    inline bool operator<(const AStarHeapData& lhs, const AStarHeapData& rhs)
    {
      return lhs.priority > rhs.priority;
    }

    using AStarHeap = BinaryHeap<AStarHeapData>;

    enum class AStarState : uint8_t {
      None,
      Open,
      Closed,
    };

    struct AStarCellData {
      float distance = std::numeric_limits<float>::infinity();
      Vec2I previous = vec(-1, -1);
      AStarState state = AStarState::None;
      AStarHeap::handle_type handle = {};
    };

    using AStarData = Array2D<AStarCellData>;

  }

  class GF_CORE_API AStarAlgorithm {
  public:

    template<typename Cell, typename Grid, typename CostFunction>
    std::vector<Vec2I> operator()(const Array2D<Cell>& cells, const Grid& grid, Vec2I origin, Vec2I target, CostFunction cost_function, Flags<CellNeighborQuery> flags)
    {
      initialize(cells, origin);

      while (!m_heap.empty()) {
        const details::AStarHeapData data = m_heap.top();
        m_heap.pop();
        assert(m_data(data.position).state == details::AStarState::Open);

        if (data.position == target) {
          break;
        }

        compute_node(cells, grid, data, cost_function, flags);
      }

      return compute_route(origin, target);
    }

  private:
    template<typename Cell>
    void initialize(const Array2D<Cell>& cells, Vec2I origin)
    {
      m_data = details::AStarData(cells.size());
      m_heap.clear();

      m_data(origin).distance = 0.0f;
      m_data(origin).state = details::AStarState::Open;

      m_heap.push({ origin, 0.0f });
    }

    template<typename Cell, typename Grid, typename CostFunction>
    void compute_node(const Array2D<Cell>& cells, const Grid& grid,  details::AStarHeapData heap_data, CostFunction cost_function, Flags<CellNeighborQuery> flags)
    {
      m_data(heap_data.position).state = details::AStarState::Closed;

      const std::vector<Vec2I> neighbors = grid.compute_neighbors(heap_data.position, flags);

      for (auto position : neighbors) {
        assert(position != heap_data.position);

        const Cell& cell = cells(position);

        if (!cell.walkable()) {
          continue;
        }

        if (m_data(position).state == details::AStarState::Closed) {
          continue;
        }

        const float updated_distance = m_data(heap_data.position).distance + cost_function(heap_data.position, position);

        if (updated_distance < m_data(position).distance) {
          details::AStarCellData& data = m_data(position);
          data.distance = updated_distance;
          data.previous = heap_data.position;

          const float priority = updated_distance + (compute_heuristic(heap_data.position, position, flags) * 1.001f);

          if (data.state == details::AStarState::Open) {
            assert(m_heap(data.handle).position == position);

            if (m_heap(data.handle).priority != priority) {
              m_heap(data.handle).priority = priority;
              m_heap.increase(data.handle);
            }
          } else {
            assert(data.state == details::AStarState::None);
            data.handle = m_heap.push({ position, priority });
            data.state = details::AStarState::Open;
          }
        }
      }
    }

    static float compute_heuristic(Vec2I position, Vec2I neighbor, Flags<CellNeighborQuery> flags)
    {
      if (!flags.test(CellNeighborQuery::Diagonal)) {
        return 1.0f * static_cast<float>(manhattan_distance(position, neighbor));
      }

      Vec2F d = gf::abs(position - neighbor);
      return (1.0f * (d.x + d.y)) + ((Sqrt2 - 2.0f) * std::min(d.x, d.y));
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

    details::AStarData m_data;
    details::AStarHeap m_heap;
  };


  template<typename Cell, typename Grid, typename CostFunction>
  std::vector<Vec2I> compute_route_astar(const Array2D<Cell>& cells, const Grid& grid, Vec2I origin, Vec2I target, CostFunction cost_function, Flags<CellNeighborQuery> flags)
  {
    AStarAlgorithm algorithm = {};
    return algorithm(cells, grid, origin, target, cost_function, flags);
  }

}

#endif // GF_PATH_FINDING_H

