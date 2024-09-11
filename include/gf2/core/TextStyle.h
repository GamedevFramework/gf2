// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TEXT_STYLE_H
#define GF_TEXT_STYLE_H

#include <cstdint>

#include <map>

#include "Color.h"
#include "CoreApi.h"
#include "Flags.h"
#include "Id.h"

namespace gf {

  enum class FontStyle : uint8_t {
    Bold = 0x01,
    Italic = 0x02,
  };

  template<>
  struct EnableBitmaskOperators<FontStyle> : std::true_type {
  };

  struct GF_CORE_API TextStyle {
    float character_size_factor = 1.0f;
    Color color = Black;
    Flags<FontStyle> font_style = None;
  };

  class GF_CORE_API TextRichStyle {
  public:
    void set_style(std::string_view name, const TextStyle& style);
    void set_style(Id id, const TextStyle& style);

    TextStyle style(std::string_view name) const;
    TextStyle style(Id id) const;

  private:
    std::map<Id, TextStyle> m_styles;
  };

}

#endif // GF_TEXT_STYLE_H
