// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_MOUSE_TYPES_H
#define GF_MOUSE_TYPES_H

#include <cstdint>

namespace gf {

  enum class MouseButton : uint32_t { // NOLINT(performance-enum-size)
    None,
    Left,
    Middle,
    Right,
    XButton1,
    XButton2,
    Touch1,
    Touch2,
  };

  constexpr MouseButton AnyMouseButton = static_cast<MouseButton>(0xFFFFFFFF);

  enum class MouseId : uint32_t;

  constexpr MouseId TouchMouseId = static_cast<MouseId>(0xFFFFFFFF);

  enum class MouseWheelDirection : uint32_t { // NOLINT(performance-enum-size)
    Normal,
    Flipped,
  };

} // namespace gf

#endif // GF_MOUSE_TYPES_H
