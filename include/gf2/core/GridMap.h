// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GRID_MAP_H
#define GF_GRID_MAP_H

#include <functional>
#include <vector>

#include "AnyGrid.h"
#include "Array2D.h"
#include "CoreApi.h"
#include "Flags.h"
#include "Math.h"

namespace gf {

  enum class Route : uint8_t {
    AStar,
    Dijkstra,
  };

  struct GF_CORE_API RouteCost {
    float cardinal = 1.0f;
    float diagonal = Sqrt2;
    float blocked = 5.0f;
  };

  using RouteCostFunction = std::function<float(Vec2I position, Vec2I neighbor, Flags<CellProperty> neighbor_properties, const AnyGrid& grid)>;

  class GF_CORE_API GridMap {
  public:
    GridMap() = default;

    static GridMap make_orthogonal(Vec2I size);
    static GridMap make_orthogonal(Vec2I layer_size, Vec2I tile_size);

    static GridMap make_isometric(Vec2I size);
    static GridMap make_isometric(Vec2I layer_size, Vec2I tile_size);

    static GridMap make_staggered(Vec2I size, CellAxis axis, CellIndex index);
    static GridMap make_staggered(Vec2I layer_size, Vec2I tile_size, CellAxis axis, CellIndex index);

    static GridMap make_hexagonal(Vec2I size, CellAxis axis, CellIndex index);
    static GridMap make_hexagonal(Vec2I layer_size, Vec2I tile_size, int32_t side_length, CellAxis axis, CellIndex index);
    static GridMap make_hexagonal(Vec2I layer_size, float radius, CellAxis axis, CellIndex index);

    const AnyGrid& grid() const;

    Vec2I size() const;
    PositionRange position_range() const;
    bool valid(Vec2I position) const;

    void reset(Flags<CellProperty> properties);

    void set_properties(Vec2I position, Flags<CellProperty> properties);
    void add_properties(Vec2I position, Flags<CellProperty> properties);

    bool transparent(Vec2I position) const;
    void set_transparent(Vec2I position, bool transparent = true);

    bool walkable(Vec2I position) const;
    void set_walkable(Vec2I position, bool walkable = true);

    void set_empty(Vec2I position);

    bool blocked(Vec2I position) const;
    void set_blocked(Vec2I position, bool blocked = true);
    void clear_blocks();

    void clear_visible();
    void clear_explored();

    bool visible(Vec2I position) const;
    bool explored(Vec2I position) const;

    uint32_t tag(Vec2I position) const;
    void set_tag(Vec2I position, uint32_t tag);

    template<typename E, typename = std::enable_if_t<std::is_enum_v<E>>>
    E tag_as(Vec2I position) const
    {
      return static_cast<E>(static_cast<std::underlying_type_t<E>>(tag(position)));
    }

    template<typename E, typename = std::enable_if_t<std::is_enum_v<E>>>
    void set_tag(Vec2I position, E tag)
    {
      set_tag(position, static_cast<uint32_t>(static_cast<std::underlying_type_t<E>>(tag)));
    }

    std::vector<Vec2I> compute_route(Vec2I origin, Vec2I target, RouteCost cost = {}, Route route = Route::AStar);
    std::vector<Vec2I> compute_route(Vec2I origin, Vec2I target, RouteCostFunction function, Flags<CellNeighborQuery> flags = CellNeighborQuery::Valid, Route route = Route::AStar);

    void compute_field_of_vision(Vec2I origin, int range_limit);
    void compute_local_field_of_vision(Vec2I origin, int range_limit);

  private:
    GridMap(Vec2I size, AnyGrid grid);

    void raw_compute_field_of_vision(Vec2I origin, int range_limit, Flags<CellProperty> properties);

    Array2D<Flags<CellProperty>> m_cells;
    Array2D<uint32_t> m_tags;
    AnyGrid m_grid;
  };

}

#endif // GF_GRID_MAP_H
