// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/core/Direction.h>
// clang-format on

#include <cassert>

#include <limits>

#include <gf2/core/Math.h>

namespace gf {

  Vec2F unit(Direction direction)
  {
    switch (direction) {
      case Direction::Center:
        return { +0.0f, +0.0f };
      case Direction::Up:
        return { +0.0f, -1.0f };
      case Direction::Right:
        return { +1.0f, +0.0f };
      case Direction::Down:
        return { +0.0f, +1.0f };
      case Direction::Left:
        return { -1.0f, +0.0f };
    }

    assert(false);
    return { 0.0f, 0.0f };
  }

  Vec2I displacement(Direction direction)
  {
    switch (direction) {
      case Direction::Center:
        return { +0, +0 };
      case Direction::Up:
        return { +0, -1 };
      case Direction::Right:
        return { +1, +0 };
      case Direction::Down:
        return { +0, +1 };
      case Direction::Left:
        return { -1, +0 };
    }

    assert(false);
    return { 0, 0 };
  }

  float angle(Direction direction)
  {
    if (direction == Direction::Center) {
      return std::numeric_limits<float>::quiet_NaN();
    }

    return static_cast<float>(direction) * Pi / 2;
  }

  Direction opposite(Direction direction)
  {
    if (direction == Direction::Center) {
      return Direction::Center;
    }

    auto val = static_cast<int8_t>(direction);
    return static_cast<Direction>((val + 2) % 4);
  }

} // namespace gf
