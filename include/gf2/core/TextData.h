// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TEXT_DATA_H
#define GF_TEXT_DATA_H

#include <cstdint>

#include <filesystem>
#include <string>

#include "Color.h"

namespace gf {

  enum class Alignment : uint8_t {
    None,
    Left,
    Right,
    Center,
    Justify,
  };

  struct TextData {
    std::string content;
    uint32_t character_size = 0;
    Color color = Black;
    float outline_thickness = 0.0f;
    Color outline_color = White;
    Alignment alignment = Alignment::None;
    float paragraph_width = 0.0f;
    float line_spacing_factor = 1.0f;
    float letter_spacing_factor = 1.0f;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, TextData& data)
  {
    return ar | data.content | data.character_size | data.color | data.outline_thickness | data.outline_color | data.alignment | data.paragraph_width | data.line_spacing_factor | data.letter_spacing_factor;
  }

  struct TextResource {
    std::filesystem::path font;
    TextData data;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, TextResource& resource)
  {
    return ar | resource.font | resource.data;
  }

}

#endif // GF_TEXT_DATA_H
