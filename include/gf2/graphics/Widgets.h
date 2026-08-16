// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_WIDGETS_H
#define GF_WIDGETS_H

#include <gf2/core/WidgetData.h>

#include "FontAtlas.h"
#include "GraphicsApi.h"
#include "Shape.h"
#include "Sprite.h"
#include "Text.h"
#include "Widget.h"

namespace gf {
  class RenderManager;
  class ResourceManager;

  class GF_GRAPHICS_API TextWidget : public Widget {
  public:
    TextWidget(FontAtlas* atlas, FontFace* face, const TextWidgetData& data, RenderManager* render_manager);
    TextWidget(FontAtlas* atlas, const TextWidgetResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    bool contains(Vec2F pointer) override;
    void render(RenderRecorder& recorder) override;

  protected:
    RectF bounds() const;
    Text& current_text();
    void render_text(RenderRecorder& recorder);

  private:
    Text m_disabled_text;
    Text m_default_text;
    Text m_selected_text;
  };


  class GF_GRAPHICS_API TextButtonWidget : public TextWidget {
  public:
    TextButtonWidget(FontAtlas* atlas, FontFace* face, const TextButtonWidgetData& data, RenderManager* render_manager);
    TextButtonWidget(FontAtlas* atlas, const TextButtonWidgetResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    bool contains(Vec2F pointer) override;
    void render(RenderRecorder& recorder) override;

  private:
    RectF bounds() const;
    Shape& current_shape();
    void render_shape(RenderRecorder& recorder);

    Shape m_disabled_shape;
    Shape m_default_shape;
    Shape m_selected_shape;
  };

  class GF_GRAPHICS_API SpriteWidget : public Widget {
  public:
    SpriteWidget(GpuTexture* disabled_texture, GpuTexture* default_texture, GpuTexture* selected_texture, const SpriteWidgetData& data, RenderManager* render_manager);
    SpriteWidget(const SpriteWidgetResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    bool contains(Vec2F pointer) override;
    void render(RenderRecorder& recorder) override;

  private:
    RectF bounds() const;
    Sprite& current_sprite();

    Sprite m_disabled_sprite;
    Sprite m_default_sprite;
    Sprite m_selected_sprite;
  };

}

#endif // GF_WIDGETS_H
