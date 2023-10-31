// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_FONT_PACK_H
#define GF_FONT_PACK_H

#include <filesystem>

#include <gf2/core/Font.h>

#include "FontAtlas.h"
#include "GraphicsApi.h"

namespace gf {

  struct GF_GRAPHICS_API FontPackContext {
    FontManager* font_manager = nullptr;
    RenderManager* render_manager = nullptr;
    Vec2I size = { 2048, 2048 };
  };

  class GF_GRAPHICS_API FontPack {
  public:
    using Context = FontPackContext;

    FontPack(const std::filesystem::path& filename, FontPackContext context);

    Font* font()
    {
      return &m_font;
    }

    FontAtlas* atlas()
    {
      return &m_atlas;
    }

  private:
    Font m_font;
    FontAtlas m_atlas;
  };

}

#endif // GF_FONT_PACK_H

