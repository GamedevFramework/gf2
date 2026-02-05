// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_FONT_DATA_H
#define GF_CONSOLE_FONT_DATA_H

#include <cstdint>

#include "CoreApi.h"
#include "TypeTraits.h"
#include "Vec2.h"

namespace gf {

  enum class ConsoleFontTransparency : uint8_t {
    Alpha,
    Grayscale,
    ColorKey,
  };

  enum class ConsoleFontLayout : uint8_t {
    InColumn,
    InRow,
  };

  enum class ConsoleFontMapping : uint8_t {
    CodePage437,
    ModifiedCodePage437,
    Special,
    Custom,
  };

  struct GF_CORE_API ConsoleFontFormat {
    ConsoleFontTransparency transparency;
    ConsoleFontLayout layout;
    ConsoleFontMapping mapping;
  };

  inline constexpr ConsoleFontFormat LibtcodFormat = { ConsoleFontTransparency::ColorKey, ConsoleFontLayout::InRow, ConsoleFontMapping::Special };
  inline constexpr ConsoleFontFormat DwarfFortressFormat = { ConsoleFontTransparency::ColorKey, ConsoleFontLayout::InRow, ConsoleFontMapping::CodePage437 };

  struct GF_CORE_API ConsoleFontData {
    ConsoleFontFormat font_format;
    Vec2I font_size = { 0, 0 };
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ConsoleFontData, Archive>& data)
  {
    return ar | data.font_format.transparency | data.font_format.layout | data.font_format.mapping | data.font_size;
  }

}

#endif // GF_CONSOLE_FONT_FONT_DATA_H
