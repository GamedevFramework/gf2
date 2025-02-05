// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_MOUSE_TYPES_H
#define GF_MOUSE_TYPES_H

#include <cstdint>

#include <limits>
#include <type_traits>

namespace gf {

  enum class MouseButton : uint8_t {
    None,
    Left,
    Middle,
    Right,
    XButton1,
    XButton2,
    Touch1,
    Touch2,
  };

  constexpr MouseButton AnyMouseButton = MouseButton{ std::numeric_limits<std::underlying_type_t<MouseButton>>::max() };

  enum class MouseId : uint32_t;

  constexpr MouseId TouchMouseId = MouseId{ std::numeric_limits<std::underlying_type_t<MouseId>>::max() };

  enum class MouseWheelDirection { // NOLINT(performance-enum-size)
    Normal,
    Flipped,
  };

} // namespace gf

#endif // GF_MOUSE_TYPES_H
