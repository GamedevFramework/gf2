// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TEXT_ENTITY_H
#define GF_TEXT_ENTITY_H

#include <gf2/core/Transform.h>

#include "GraphicsApi.h"
#include "Text.h"
#include "TransformableEntity.h"

namespace gf {
  class RenderManager;
  class ResourceManager;

  class GF_GRAPHICS_API TextEntity : public TransformableEntity {
  public:
    TextEntity(FontAtlas* atlas, FontFace* face, const TextData& data, RenderManager* render_manager);
    TextEntity(FontAtlas* atlas, const TextResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    Text& text()
    {
      return m_text;
    }

    void render(RenderRecorder& recorder) override;

  private:
    Text m_text;
  };

  class GF_GRAPHICS_API RichTextEntity : public TransformableEntity {
  public:
    RichTextEntity(FontAtlas* atlas, RichTextStyle* style, FontFace* default_font, FontFace* bold_font, FontFace* italic_font, FontFace* bold_italic_font, const TextData& data, RenderManager* render_manager);
    RichTextEntity(FontAtlas* atlas, RichTextStyle* style, const RichTextResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    RichText& text()
    {
      return m_text;
    }

    void render(RenderRecorder& recorder) override;

  private:
    RichText m_text;
  };

}

#endif // GF_TEXT_ENTITY_H
