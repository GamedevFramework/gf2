// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_FONT_ATLAS_H
#define GF_FONT_ATLAS_H

#include <map>

#include <gf2/core/BinPack.h>
#include <gf2/core/Bitmap.h>
#include <gf2/core/Fixed.h>
#include <gf2/core/FontFace.h>
#include <gf2/core/Vec2.h>

#include "GraphicsApi.h"
#include "Texture.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API FontAtlas {
  public:
    FontAtlas(FontFace* face, Vec2I size, RenderManager* render_manager);

    RectF texture_region(char32_t codepoint);
    void update_texture_regions_for(std::string_view string);

    const FontGlyph& glyph(char32_t codepoint);

    void update_texture(RenderManager* render_manager);

    float kerning(char32_t left, char32_t right)
    {
      return m_face->compute_kerning(left, right);
    }

    float line_spacing()
    {
      return m_face->compute_line_spacing();
    }

    const Texture* texture() const
    {
      return &m_texture;
    }

  private:
    FontFace* m_face = nullptr;
    BinPack m_bin_pack;
    Bitmap m_bitmap;
    Texture m_texture;

    using AtlasKey = char32_t;

    std::map<AtlasKey, RectF> m_atlas;
    std::map<AtlasKey, FontGlyph> m_glyph_cache;
  };

}

#endif // GF_FONT_ATLAS_H
