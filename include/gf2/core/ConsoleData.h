// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_DATA_H
#define GF_CONSOLE_DATA_H

#include <cstdint>

#include <filesystem>

#include "CoreApi.h"
#include "TypeTraits.h"
#include "Vec2.h"

namespace gf {

  struct GF_CORE_API ConsoleFontFormat {
    enum Transparency : uint8_t {
      Alpha,
      Grayscale,
      ColorKey,
    };

    Transparency transparency;

    enum Layout : uint8_t {
      InColumn,
      InRow,
    };

    Layout layout;

    enum Mapping : uint8_t {
      CodePage437,
      ModifiedCodePage437,
      Special,
      Custom,
    };

    Mapping mapping;
  };

  inline constexpr ConsoleFontFormat LibtcodFormat = { ConsoleFontFormat::ColorKey, ConsoleFontFormat::InRow, ConsoleFontFormat::Special };
  inline constexpr ConsoleFontFormat DwarfFortressFormat = { ConsoleFontFormat::ColorKey, ConsoleFontFormat::InRow, ConsoleFontFormat::CodePage437 };

  struct GF_CORE_API ConsoleData {
    ConsoleFontFormat font_format;
    Vec2I font_size = { 0, 0 };
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ConsoleData, Archive>& data)
  {
    return ar | data.font_format.transparency | data.font_format.layout | data.font_format.mapping | data.font_size;
  }

  struct GF_CORE_API ConsoleResource {
    std::filesystem::path console_font;
    ConsoleData data;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ConsoleResource, Archive>& resource)
  {
    return ar | resource.console_font | resource.data;
  }

}

#endif // GF_CONSOLE_FONT_DATA_H
