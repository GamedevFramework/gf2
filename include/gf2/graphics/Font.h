// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_FONT_H
#define GF_FONT_H

#include <filesystem>

#include <gf2/core/FontFace.h>

#include "FontAtlas.h"
#include "GraphicsApi.h"

namespace gf {

  struct GF_GRAPHICS_API FontContext {
    FontManager* font_manager = nullptr;
    RenderManager* render_manager = nullptr;
    Vec2I size = { 2048, 2048 };
  };

  class GF_GRAPHICS_API Font {
  public:
    using Context = FontContext;

    Font(const std::filesystem::path& filename, FontContext context);

    FontFace* face()
    {
      return &m_face;
    }

    FontAtlas* atlas()
    {
      return &m_atlas;
    }

  private:
    FontFace m_face;
    FontAtlas m_atlas;
  };

}

#endif // GF_FONT_H

