// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/TextStyle.h>

namespace gf {

  void TextRichStyle::set_style(std::string_view name, const TextStyle& style)
  {
    set_style(hash_string(name), style);
  }

  void TextRichStyle::set_style(Id id, const TextStyle& style)
  {
    m_styles.emplace(id, style);
  }

  TextStyle TextRichStyle::style(std::string_view name) const
  {
    return style(hash_string(name));
  }

  TextStyle TextRichStyle::style(Id id) const
  {
    if (auto iterator = m_styles.find(id); iterator != m_styles.end()) {
      return iterator->second;
    }

    return {};
  }

}
