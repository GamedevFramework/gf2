// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TEXT_H
#define GF_TEXT_H

#include <gf2/core/Rect.h>
#include <gf2/core/TextData.h>
#include <gf2/core/TextStyle.h>

#include "Buffer.h"
#include "DynamicBuffer.h"
#include "FontAtlas.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class RenderManager;
  class ResourceManager;

  class GF_GRAPHICS_API Text {
  public:
    Text(FontAtlas* atlas, FontFace* default_font, const TextData& data, RenderManager* render_manager);
    Text(FontAtlas* atlas, const TextResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    void update(const TextData& data, RenderManager* render_manager);

    RenderGeometry geometry() const;

    const Texture* texture() const
    {
      return m_atlas->texture();
    }

    RectF bounds() const
    {
      return m_bounds;
    }

  private:

    FontAtlas* m_atlas = nullptr;
    FontFace* m_font = nullptr;
    DynamicBuffer m_vertices;
    DynamicBuffer m_indices;
    RectF m_bounds = {};
  };

  class GF_GRAPHICS_API RichText {
  public:
    RichText(FontAtlas* atlas, RichTextStyle* style, FontFace* default_font, FontFace* bold_font, FontFace* italic_font, FontFace* bold_italic_font, const TextData& data, RenderManager* render_manager);
    RichText(FontAtlas* atlas, RichTextStyle* style, const RichTextResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    void update(const TextData& data, RenderManager* render_manager);

    RenderGeometry geometry() const;

    const Texture* texture() const
    {
      return m_atlas->texture();
    }

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    FontAtlas* m_atlas = nullptr;
    RichTextStyle* m_style = nullptr;
    FontFace* m_default_font = nullptr;
    FontFace* m_bold_font = nullptr;
    FontFace* m_italic_font = nullptr;
    FontFace* m_bold_italic_font = nullptr;
    DynamicBuffer m_vertices;
    DynamicBuffer m_indices;
    RectF m_bounds = {};
  };

}

#endif // GF_TEXT_H
