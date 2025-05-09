// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_TEXT_DATA_H
#define GF_TEXT_DATA_H

#include <cstdint>

#include <filesystem>
#include <string>

#include "Color.h"
#include "CoreApi.h"
#include "TypeTraits.h"

namespace gf {

  enum class Alignment : uint8_t {
    None,
    Left,
    Right,
    Center,
    Justify,
  };

  struct GF_CORE_API TextData {
    std::string content;
    float character_size = 0.0f;
    Color color = Black;
    Color outline_color = Transparent;
    float outline_thickness = 0.0f;
    Alignment alignment = Alignment::None;
    float paragraph_width = 0.0f;
    float line_spacing_factor = 1.0f;
    float letter_spacing_factor = 1.0f;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TextData, Archive>& data)
  {
    return ar | data.content | data.character_size | data.color | data.outline_color | data.outline_thickness | data.alignment | data.paragraph_width | data.line_spacing_factor | data.letter_spacing_factor;
  }

  struct GF_CORE_API TextResource {
    std::filesystem::path font;
    TextData data;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TextResource, Archive>& resource)
  {
    return ar | resource.font | resource.data;
  }

  struct GF_CORE_API RichTextResource {
    std::filesystem::path default_font;
    std::filesystem::path bold_font;
    std::filesystem::path italic_font;
    std::filesystem::path bold_italic_font;
    TextData data;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<RichTextResource, Archive>& resource)
  {
    return ar | resource.default_font | resource.bold_font | resource.italic_font | resource.bold_italic_font | resource.data;
  }

}

#endif // GF_TEXT_DATA_H
