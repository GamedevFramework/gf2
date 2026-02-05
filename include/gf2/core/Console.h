// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_H
#define GF_CONSOLE_H

#include <cstdint>

#include "Array2D.h"
#include "Color.h"
#include "CoreApi.h"
#include "TypeTraits.h"

namespace gf {

  enum class ConsoleMode : uint8_t {
    Picture = 1,
    Text = 2,
  };

  struct GF_CORE_API ConsoleCellPart {
    Color foreground = White;
    Color background = Black;
    char16_t character = u' ';
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<ConsoleCellPart, Archive>& cell)
  {
    return ar | cell.foreground | cell.background | cell.character;
  }

  struct GF_CORE_API ConsoleCell {
    ConsoleMode mode = ConsoleMode::Picture;
    std::array<ConsoleCellPart, 2> parts;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<ConsoleCell, Archive>& cell)
  {
    switch (cell.mode) {
      case ConsoleMode::Picture:
        ar | cell.parts[0];
        break;
      case ConsoleMode::Text:
        ar | cell.parts[0] | cell.parts[1];
        break;
    }

    return ar;
  }

  using Console = Array2D<ConsoleCell>;

}

#endif // GF_CONSOLE_H
