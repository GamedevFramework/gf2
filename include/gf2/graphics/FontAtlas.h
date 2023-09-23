// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_FONT_ATLAS_H
#define GF_FONT_ATLAS_H

#include <cstdint>

#include <map>

#include <gf2/core/BinPack.h>
#include <gf2/core/Bitmap.h>
#include <gf2/core/Fixed.h>
#include <gf2/core/Font.h>
#include <gf2/core/Vec2.h>

#include "GraphicsApi.h"
#include "Texture.h"

namespace gf {
  class Renderer;

  class GF_GRAPHICS_API FontAtlas {
  public:
    FontAtlas(Font* font, Vec2I size, Renderer* renderer);

    RectF texture_region(char32_t codepoint, uint32_t character_size, float outline_thickness = 0.0f);
    void update_texture_regions_for(std::string_view string, uint32_t character_size, float outline_thickness = 0.0f);

    const FontGlyph& glyph(char32_t codepoint, uint32_t character_size, float outline_thickness = 0.0f);

    void update_texture(Renderer* renderer);

    float kerning(char32_t left, char32_t right, uint32_t character_size)
    {
      return m_font->compute_kerning(left, right, character_size);
    }

    float line_spacing(uint32_t character_size)
    {
      return m_font->compute_line_spacing(character_size);
    }

    const Texture* texture() const
    {
      return &m_texture;
    }

  private:
    Font* m_font = nullptr;
    BinPack m_bin_pack;
    Bitmap m_bitmap;
    Texture m_texture;

    using AtlasKey = std::tuple<char32_t, uint32_t, Fixed32>;

    std::map<AtlasKey, RectF> m_atlas;
    std::map<AtlasKey, FontGlyph> m_glyph_cache;
  };

}

#endif // GF_FONT_ATLAS_H
