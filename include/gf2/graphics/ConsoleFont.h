// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_FONT_H
#define GF_CONSOLE_FONT_H

#include <cstdint>

#include <filesystem>

#include <gf2/core/ConsoleFontData.h>
#include <gf2/core/Span.h>

#include "GraphicsApi.h"
#include "GpuTexture.h"

namespace gf {

  struct GF_GRAPHICS_API ConsoleFontContext {
    ConsoleFontFormat format;
    Vec2I size = { 0, 0 };
    RenderManager* render_manager = nullptr;
  };

  class GF_GRAPHICS_API ConsoleFont {
  public:
    using Context = ConsoleFontContext;

    ConsoleFont();
    ConsoleFont(const std::filesystem::path& filename, ConsoleFontContext context);
    ConsoleFont(const Image& image, ConsoleFontFormat format, Vec2I size, RenderManager* render_manager);

    ConsoleFontFormat format() const
    {
      return m_format;
    }

    Vec2I size() const
    {
      return m_size;
    }

    Vec2I character_size() const
    {
      return m_character_size;
    }

    void map_code(char16_t character, Vec2I position);
    void map_code_range(char16_t character, int count, Vec2I position);
    void map_string(std::string_view str, Vec2I position);
    void map_element(ConsoleFontElement element);
    void map_elements(Span<const ConsoleFontElement> elements);
    void clear_mapping();

    RectF texture_region(char16_t character) const;

    GpuTexture* texture()
    {
      return &m_texture;
    }

  private:
    Vec2I color_key_position() const;

    std::vector<uint8_t> m_mapping;
    ConsoleFontFormat m_format;
    Vec2I m_size;
    Vec2I m_character_size;
    GpuTexture m_texture;
  };

}

#endif // GF_CONSOLE_FONT_H
