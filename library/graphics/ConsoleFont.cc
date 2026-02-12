// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/ConsoleFont.h>

#include <cstdint>

#include <algorithm>

#include <gf2/core/Log.h>
#include <gf2/core/StringUtils.h>

namespace gf {

  namespace {

    int compute_index(Vec2I position, ConsoleFontLayout layout, Vec2I size)
    {
      switch (layout) {
        case ConsoleFontLayout::InColumn:
          return (position.x * size.h) + position.y;
        case ConsoleFontLayout::InRow:
          return (position.y * size.w) + position.x;
      }

      assert(false);
      return 0;
    }

    Vec2I compute_position(uint8_t index, ConsoleFontLayout layout, Vec2I size)
    {
      switch (layout) {
        case ConsoleFontLayout::InColumn:
          return { index / size.h, index % size.h };
        case ConsoleFontLayout::InRow:
          return { index % size.w, index / size.w };
      }

      assert(false);
      return { 0, 0 };
    }

    constexpr std::size_t MappingSize = 0x10000;

  }

  ConsoleFont::ConsoleFont()
  : m_mapping(MappingSize, 0x00)
  , m_format({ ConsoleFontTransparency::Alpha, ConsoleFontLayout::InRow, ConsoleFontMapping::Custom })
  , m_size(0, 0)
  , m_character_size(0, 0)
  {
  }

  ConsoleFont::ConsoleFont(const std::filesystem::path& filename, ConsoleFontContext context)
  : ConsoleFont(Image(filename), context.format, context.size, context.render_manager)
  {
  }

  ConsoleFont::ConsoleFont(const Image& image, ConsoleFontFormat format, Vec2I size, RenderManager* render_manager)
  : m_mapping(MappingSize, 0x00)
  , m_format(format)
  , m_size(size)
  , m_character_size(0, 0)
  {
    switch (format.mapping) {
      case ConsoleFontMapping::CodePage437:
        map_elements(load_console_font_mapping(ConsoleFontMapping::CodePage437));
        break;
      case ConsoleFontMapping::ModifiedCodePage437:
        map_elements(load_console_font_mapping(ConsoleFontMapping::CodePage437));
        map_elements(load_console_font_mapping(ConsoleFontMapping::ModifiedCodePage437));
        break;
      case ConsoleFontMapping::Special:
        map_elements(load_console_font_mapping(ConsoleFontMapping::Special));
        break;
      case ConsoleFontMapping::Custom:
        // nothing to map
        break;
    }

    if (size == gf::vec(0, 0)) {
      switch (format.mapping) {
        case ConsoleFontMapping::CodePage437:
        case ConsoleFontMapping::ModifiedCodePage437:
          m_size = { 16, 16 };
          break;

        case ConsoleFontMapping::Special:
          m_size = { 32, 8 };
          break;

        case ConsoleFontMapping::Custom:
          Log::fatal("Undefined size for a custom font");
      }
    }

    auto image_size = image.size();

    if (image_size.w % m_size.w != 0 || image_size.h % m_size.h != 0) {
      Log::fatal("Image size ({}x{}) is not a multiple of font layout ({}x{})", image_size.w, image_size.h, m_size.w, m_size.h);
    }

    m_character_size = image_size / m_size;

    Image copy(image);

    switch (format.transparency) {
      case ConsoleFontTransparency::Alpha:
        // nothing to do
        break;

      case ConsoleFontTransparency::Grayscale:
        for (auto position : copy.position_range()) {
          const Color color = copy(position);
          assert(color.r == color.g && color.g == color.b);
          copy.put_pixel(position, opaque(color.r));
        }
        break;

      case ConsoleFontTransparency::ColorKey:
        {
          const Color key = copy(color_key_position());

          for (auto position : copy.position_range()) {
            const Color color = copy(position);

            if (color == key) {
              copy.put_pixel(position, Transparent);
            }
          }
        }
        break;
    }

    m_texture = { copy, render_manager };
  }

  void ConsoleFont::map_code(char16_t character, Vec2I position)
  {
    const int index = compute_index(position, m_format.layout, m_size);
    assert(0 <= index && index < 0x100);
    m_mapping[character] = static_cast<uint8_t>(index);
  }

  void ConsoleFont::map_code_range(char16_t character, int count, Vec2I position)
  {
    int index = compute_index(position, m_format.layout, m_size);

    for (int i = 0; i < count; ++i) {
      assert(0 <= index && index < 0x100);
      m_mapping[character++] = static_cast<uint8_t>(index);
      ++index;
    }
  }

  void ConsoleFont::map_string(std::string_view str, Vec2I position)
  {
    int index = compute_index(position, m_format.layout, m_size);

    for (auto codepoint : gf::codepoints(str)) {
      if (codepoint >= 0x10000) {
        auto value = static_cast<std::uint_least32_t>(codepoint);
        Log::error("Cannot map a codepoint outside the Basic Multilingual Plane (BMP): U+{:X}", value);
      } else {
        auto character = static_cast<char16_t>(codepoint);
        assert(0 <= index && index < 0x100);
        m_mapping[character] = static_cast<uint8_t>(index);
      }

      ++index;
    }
  }

  void ConsoleFont::map_element(ConsoleFontElement element)
  {
    m_mapping[element.character] = element.index;
  }

  void ConsoleFont::map_elements(Span<const ConsoleFontElement> elements)
  {
    for (const ConsoleFontElement element : elements) {
      m_mapping[element.character] = element.index;
    }
  }

  void ConsoleFont::clear_mapping()
  {
    std::ranges::fill(m_mapping, 0x00);
    assert(m_mapping.size() == MappingSize);
  }

  RectF ConsoleFont::texture_region(char16_t character) const
  {
    const uint8_t index = m_mapping[character];
    const Vec2I position = compute_position(index, m_format.layout, m_size);
    const Vec2F size = m_size;
    return RectF::from_position_size(position / size, 1.0f / size);
  }

  Vec2I ConsoleFont::color_key_position() const
  {
    switch (m_format.mapping) {
      case ConsoleFontMapping::CodePage437:
      case ConsoleFontMapping::ModifiedCodePage437:
        // space character at index 0x20
        return compute_position(0x20, m_format.layout, m_size) * m_character_size;
      case ConsoleFontMapping::Special:
        // space character at index 0
      case ConsoleFontMapping::Custom:
        // assume the top left pixel is the color key
        return { 0, 0 };
    }

    assert(false);
    return { 0, 0 };
  }

}
