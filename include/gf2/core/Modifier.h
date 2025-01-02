// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CORE_MODIFIER_H
#define GF_CORE_MODIFIER_H

#include <cstdint>

#include "Flags.h"

namespace gf {

  enum class Modifier : uint16_t { // NOLINT(performance-enum-size)
    None = 0x0000,
    LeftShift = 0x0001,
    RightShift = 0x0002,
    LeftCtrl = 0x0040,
    RightCtrl = 0x0080,
    LeftAlt = 0x0100,
    RightAlt = 0x0200,
    LeftGui = 0x0400,
    RightGui = 0x0800,
    Num = 0x1000,
    Caps = 0x2000,
    Mode = 0x4000,
    Shift = LeftShift | RightShift,
    Ctrl = LeftCtrl | RightCtrl,
    Alt = LeftAlt | RightAlt,
    Gui = LeftGui | RightGui,
  };

  template<>
  struct EnableBitmaskOperators<Modifier> : std::true_type {
  };

}

#endif // GF_CORE_MODIFIER_H
