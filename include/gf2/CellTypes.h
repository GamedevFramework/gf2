// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CELLS_TYPES_H
#define GF_CELLS_TYPES_H

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
    Unknown = 0x00,
    Orthogonal = 0x01,
    Isometric = 0x02,
    Staggered = 0x03,
    Hexagonal = 0x04,
  };

  enum class Flip : uint8_t {
    Horizontally = 0x01,
    Vertically = 0x02,
    Diagonally = 0x04,
    Rotation60 = 0x04,
    Rotation120 = 0x08,
  };

} // namespace gf

#endif // GF_CELLS_TYPES_H
