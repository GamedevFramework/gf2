// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_WIDGETS_H
#define GF_WIDGETS_H

#include <gf2/core/WidgetData.h>

#include "FontAtlas.h"
#include "GraphicsApi.h"
#include "Text.h"
#include "Widget.h"

namespace gf {
  class RenderManager;
  class ResourceManager;

  class GF_GRAPHICS_API TextWidget : public Widget {
  public:
    TextWidget(FontAtlas* atlas, FontFace* face, const TextWidgetData& data, RenderManager* render_manager);
    TextWidget(FontAtlas* atlas, const TextWidgetResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    Text& disabled_text()
    {
      return m_disabled_text;
    }

    Text& default_text()
    {
      return m_default_text;
    }

    Text& selected_text()
    {
      return m_selected_text;
    }

    bool contains(Vec2F pointer) override;
    void render(RenderRecorder& recorder) override;

  private:
    RectF bounds() const;
    Text& current_text();

    Text m_disabled_text;
    Text m_default_text;
    Text m_selected_text;
  };

}

#endif // GF_WIDGETS_H
