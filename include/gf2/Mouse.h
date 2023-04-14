// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_MOUSE_H
#define GF_MOUSE_H

#include <cstdint>

namespace gf {

  enum class MouseButton : uint32_t {
    None,
    Left,
    Middle,
    Right,
    XButton1,
    XButton2,
    Touch1,
    Touch2,
  };

  enum class MouseId : uint32_t;

  constexpr MouseId TouchMouseId = MouseId{ 0xFFFFFFFF };

} // namespace gf

#endif // GF_MOUSE_H
