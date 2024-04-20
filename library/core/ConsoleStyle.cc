// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/ConsoleStyle.h>

namespace gf {

  ConsoleRichStyle::ConsoleRichStyle(const ConsoleStyle& default_style)
  : m_default_style(default_style)
  {
  }

  void ConsoleRichStyle::set_default_style(const ConsoleStyle& style)
  {
    m_default_style = style;
  }

  ConsoleStyle ConsoleRichStyle::default_style() const
  {
    return m_default_style;
  }

  void ConsoleRichStyle::set_style(std::string_view name, const ConsoleColorStyle& style)
  {
    set_style(gf::hash_string(name), style);
  }

  void ConsoleRichStyle::set_style(Id id, const ConsoleColorStyle& style)
  {
    m_styles.emplace(id, style);
  }

  ConsoleColorStyle ConsoleRichStyle::style(std::string_view name) const
  {
    return style(gf::hash_string(name));
  }

  ConsoleColorStyle ConsoleRichStyle::style(Id id) const
  {
    if (auto iterator = m_styles.find(id); iterator != m_styles.end()) {
      return iterator->second;
    }

    return m_default_style.color;
  }

}
