// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/TextStyle.h>

#include <gf2/core/FontFace.h>
#include <gf2/core/TextData.h>

namespace gf {

  namespace {

    TextStyle compute_from(const TextData& data)
    {
      return { FontFace::size_factor(data.character_size), srgb_to_linear(data.color) };
    }

  }

  RichTextStyle::RichTextStyle(const TextStyle& default_style)
  : m_default_style(default_style)
  {
  }

  RichTextStyle::RichTextStyle(const TextData& text_data)
  : m_default_style(compute_from(text_data))
  {
  }

  void RichTextStyle::compute_default_style_from(const TextData& text_data)
  {
    m_default_style = compute_from(text_data);
  }

  void RichTextStyle::set_default_style(const TextStyle& style)
  {
    m_default_style = style;
  }

  TextStyle RichTextStyle::default_style() const
  {
    return m_default_style;
  }

  void RichTextStyle::set_style(std::string_view name, const TextStyle& style)
  {
    set_style(hash_string(name), style);
  }

  void RichTextStyle::set_style(Id id, const TextStyle& style)
  {
    m_styles.emplace(id, style);
  }

  TextStyle RichTextStyle::style(std::string_view name) const
  {
    return style(hash_string(name));
  }

  TextStyle RichTextStyle::style(Id id) const
  {
    if (auto iterator = m_styles.find(id); iterator != m_styles.end()) {
      return iterator->second;
    }

    TextStyle default_relative_style = m_default_style;
    default_relative_style.character_size_factor = 1.0f;
    return default_relative_style;
  }

}
