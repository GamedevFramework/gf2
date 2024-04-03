// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_FONT_ATLAS_H
#define GF_FONT_ATLAS_H

#include <map>
#include <utility>

#include <gf2/core/BinPack.h>
#include <gf2/core/Bitmap.h>
#include <gf2/core/Fixed.h>
#include <gf2/core/FontFace.h>
#include <gf2/core/Span.h>
#include <gf2/core/Vec2.h>

#include "GraphicsApi.h"
#include "Texture.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API FontAtlas {
  public:
    FontAtlas(Vec2I size, RenderManager* render_manager);

    RectF texture_region(uint32_t index, FontFace* face);
    void update_texture_regions_for(Span<uint32_t> indices, FontFace* face);

    const FontGlyph& glyph(uint32_t index, FontFace* face);

    void update_texture(RenderManager* render_manager);

    const Texture* texture() const
    {
      return &m_texture;
    }

  private:
    BinPack m_bin_pack;
    Bitmap m_bitmap;
    Texture m_texture;

    using AtlasKey = std::pair<uint32_t, FontFace*>;

    std::map<AtlasKey, RectF> m_atlas;
    std::map<AtlasKey, FontGlyph> m_glyph_cache;
  };

}

#endif // GF_FONT_ATLAS_H
