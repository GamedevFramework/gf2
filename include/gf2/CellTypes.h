// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CELL_TYPES_H
#define GF_CELL_TYPES_H

#include <cstdint>

namespace gf {

  enum class CellIndex : uint8_t {
    Odd,
    Even,
  };

  enum class CellAxis : uint8_t {
    X,
    Y,
  };

  enum class CellOrientation : uint8_t {
    Unknown,
    Orthogonal,
    Isometric,
    Staggered,
    Hexagonal,
  };

  enum class CellFlip : uint8_t {
    Horizontally = 0x01,
    Vertically = 0x02,
    Diagonally = 0x04,
    Rotation60 = 0x04,
    Rotation120 = 0x08,
  };

} // namespace gf

#endif // GF_CELL_TYPES_H
