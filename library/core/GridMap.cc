// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/GridMap.h>

#include <gf2/core/FieldOfVision.h>
#include <gf2/core/Log.h>
#include <gf2/core/PathFinding.h>

namespace gf {
  namespace {

    constexpr uint32_t DefaultTag = 0;

  }

  using namespace operators;

  GridMap::GridMap(Vec2I size, AnyGrid grid)
  : m_cells(size, { CellProperty::Transparent | CellProperty::Walkable })
  , m_tags(size, DefaultTag)
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

  bool GridMap::valid(Vec2I position) const
  {
    return m_cells.valid(position);
  }

  void GridMap::reset(Flags<CellProperty> properties)
  {
    for (auto& cell : m_cells) {
      cell.flags = properties;
    }
  }

  void GridMap::set_properties(Vec2I position, Flags<CellProperty> properties)
  {
    if (!m_cells.valid(position)) {
      return;
    }

    m_cells(position).flags = properties;
  }

  void GridMap::add_properties(Vec2I position, Flags<CellProperty> properties)
  {
    if (!m_cells.valid(position)) {
      return;
    }

    m_cells(position).flags |= properties;
  }

  bool GridMap::transparent(Vec2I position) const
  {
    if (!m_cells.valid(position)) {
      return false;
    }

    return m_cells(position).flags.test(CellProperty::Transparent);
  }

  void GridMap::set_transparent(Vec2I position, bool transparent)
  {
    if (!m_cells.valid(position)) {
      return;
    }

    if (transparent) {
      m_cells(position).flags.set(CellProperty::Transparent);
    } else {
      m_cells(position).flags.reset(CellProperty::Transparent);
    }
  }

  bool GridMap::walkable(Vec2I position) const
  {
    if (!m_cells.valid(position)) {
      return false;
    }

    return m_cells(position).flags.test(CellProperty::Walkable);
  }

  void GridMap::set_walkable(Vec2I position, bool walkable)
  {
    if (!m_cells.valid(position)) {
      return;
    }

    if (walkable) {
      m_cells(position).flags.set(CellProperty::Walkable);
    } else {
      m_cells(position).flags.reset(CellProperty::Walkable);
    }
  }

  void GridMap::set_empty(Vec2I position)
  {
    if (!m_cells.valid(position)) {
      return;
    }

    m_cells(position).flags |= (CellProperty::Transparent | CellProperty::Walkable);
  }

  bool GridMap::blocked(Vec2I position) const
  {
    if (!m_cells.valid(position)) {
      return false;
    }

    return m_cells(position).flags.test(CellProperty::Blocked);
  }

  void GridMap::set_blocked(Vec2I position, bool blocked)
  {
    if (!m_cells.valid(position)) {
      return;
    }

    if (blocked) {
      m_cells(position).flags.set(CellProperty::Blocked);
    } else {
      m_cells(position).flags.reset(CellProperty::Blocked);
    }
  }

  void GridMap::clear_blocks()
  {
    for (auto& cell : m_cells) {
      cell.flags.reset(CellProperty::Blocked);
    }
  }

  /*
   * field of vision
   */

  void GridMap::clear_visible()
  {
    for (auto& cell : m_cells) {
      cell.flags.reset(CellProperty::Visible);
    }
  }

  void GridMap::clear_explored()
  {
    for (auto& cell : m_cells) {
      cell.flags.reset(CellProperty::Explored);
    }
  }

  bool GridMap::visible(Vec2I position) const
  {
    if (!m_cells.valid(position)) {
      return false;
    }

    return m_cells(position).flags.test(CellProperty::Visible);
  }

  bool GridMap::explored(Vec2I position) const
  {
    if (!m_cells.valid(position)) {
      return false;
    }

    return m_cells(position).flags.test(CellProperty::Explored);
  }

  uint32_t GridMap::tag(Vec2I position) const
  {
    if (!m_tags.valid(position)) {
      return DefaultTag;
    }

    return m_tags(position);
  }

  void GridMap::set_tag(Vec2I position, uint32_t tag)
  {
    if (!m_tags.valid(position)) {
      return;
    }

    m_tags(position) = tag;
  }

  /*
   * route
   */

  std::vector<Vec2I> GridMap::compute_route(Vec2I origin, Vec2I target, RouteCost cost, Route route)
  {
    Flags<CellNeighborQuery> flags = CellNeighborQuery::Valid;

    if (cost.diagonal > 0) {
      flags |= CellNeighborQuery::Diagonal;
    }

    auto cost_function = [cost,this](Vec2I position, Vec2I neighbor)
    {
      const bool is_diagonal = m_grid.are_diagonal_neighbors(position, neighbor);
      assert(cost.diagonal > 0 || !is_diagonal);

      float neighbor_cost = is_diagonal ? cost.diagonal : cost.cardinal;

      if (m_cells(neighbor).flags.test(CellProperty::Blocked)) {
        neighbor_cost += cost.blocked;
      }

      return neighbor_cost;
    };

    return compute_route(origin, target, cost_function, flags, route);
  }

  std::vector<Vec2I> GridMap::compute_route(Vec2I origin, Vec2I target, RouteCostFunction cost_function, Flags<CellNeighborQuery> flags,  Route route)
  {
    switch (route) {
      case Route::AStar:
        return compute_route_astar(m_cells, m_grid, origin, target, std::move(cost_function), flags);
      case Route::Dijkstra:
        return compute_route_dijkstra(m_cells, m_grid, origin, target, std::move(cost_function), flags);
    }

    return {};
  }

  /*
   * field of vision
   */

  void GridMap::compute_field_of_vision(Vec2I origin, int range_limit)
  {
    raw_compute_field_of_vision(origin, range_limit, CellProperty::Visible | CellProperty::Explored);
  }

  void GridMap::compute_local_field_of_vision(Vec2I origin, int range_limit)
  {
    raw_compute_field_of_vision(origin, range_limit, CellProperty::Visible);
  }

  void GridMap::raw_compute_field_of_vision(Vec2I origin, int range_limit, Flags<CellProperty> properties)
  {
    const GridOrientation orientation = m_grid.orientation();

    if (orientation == GridOrientation::Hexagonal || orientation == GridOrientation::Unknown) {
      Log::fatal("Unsupported orientation for field of vision.");
    }

    compute_symmetric_shadowcasting(m_cells,  m_cells, origin, range_limit, [properties](Cell& cell) { cell.flags |= properties; });
  }

}
