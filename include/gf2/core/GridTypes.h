// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_GRID_TYPES_H
#define GF_GRID_TYPES_H

#include <cstdint>

#include "Flags.h"

namespace gf {

  enum class GridOrientation : uint8_t {
    Unknown,
    Orthogonal,
    Isometric,
    Staggered,
    Hexagonal,
  };

  enum class CellIndex : uint8_t {
    Odd,
    Even,
  };

  enum class CellAxis : uint8_t {
    X,
    Y,
  };

  enum class CellFlip : uint8_t {
    Horizontally = 0x01,
    Vertically = 0x02,
    Diagonally = 0x04,
    Rotation60 = 0x04,
    Rotation120 = 0x08,
  };

  template<>
  struct EnableBitmaskOperators<CellFlip> : std::true_type {
  };

  enum class CellNeighborQuery : uint8_t {
    Valid = 0x01,
    Diagonal = 0x02,
  };

  template<>
  struct EnableBitmaskOperators<CellNeighborQuery> : std::true_type {
  };

  enum class CellProperty : uint8_t {
    Transparent = 0x01,
    Walkable = 0x02,
    Blocked = 0x03,
    Visible = 0x10,
    Explored = 0x20,
  };

  template<>
  struct EnableBitmaskOperators<CellProperty> : std::true_type {
  };

} // namespace gf

#endif // GF_GRID_TYPES_H
