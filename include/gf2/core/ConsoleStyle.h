// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CONSOLE_STYLE_H
#define GF_CONSOLE_STYLE_H

#include <cstdint>

#include <map>

#include "Color.h"
#include "ConsoleEffect.h"
#include "CoreApi.h"
#include "Id.h"

namespace gf {

  enum class ConsoleAlignment : uint8_t {
    Left,
    Center,
    Right,
  };

  struct GF_CORE_API ConsoleColorStyle {
    Color foreground = White;
    Color background = Black;
  };

  struct GF_CORE_API ConsoleStyle {
    ConsoleColorStyle color;
    ConsoleEffect effect = ConsoleEffect::set();
    ConsoleAlignment alignment = ConsoleAlignment::Left;
  };

  class GF_CORE_API ConsoleRichStyle {
  public:
    ConsoleRichStyle(const ConsoleStyle& default_style = ConsoleStyle());

    void set_default_style(const ConsoleStyle& style);
    ConsoleStyle default_style() const;

    void set_style(std::string_view name, const ConsoleColorStyle& style);
    void set_style(Id id, const ConsoleColorStyle& style);

    ConsoleColorStyle style(std::string_view name) const;
    ConsoleColorStyle style(Id id) const;

  private:
    ConsoleStyle m_default_style;
    std::map<Id, ConsoleColorStyle> m_styles;
  };

}

#endif // GF_CONSOLE_STYLE_H
