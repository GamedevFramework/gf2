// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Widgets.h>

#include <gf2/graphics/RenderRecorder.h>

namespace gf {
  /*
   * TextWidget
   */

  TextWidget::TextWidget(FontAtlas* atlas, FontFace* face, const TextWidgetData& data, RenderManager* render_manager)
  : m_disabled_text(atlas, face, data.disabled_text, render_manager)
  , m_default_text(atlas, face, data.default_text, render_manager)
  , m_selected_text(atlas, face, data.selected_text, render_manager)
  {
  }

  TextWidget::TextWidget(FontAtlas* atlas, const TextWidgetResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : m_disabled_text(atlas, { resource.font, resource.data.disabled_text }, render_manager, resource_manager)
  , m_default_text(atlas, { resource.font, resource.data.default_text }, render_manager, resource_manager)
  , m_selected_text(atlas, { resource.font, resource.data.selected_text }, render_manager, resource_manager)
  {
  }

  bool TextWidget::contains(Vec2F pointer)
  {
    const RectF object_bounds = bounds();
    const Transform object_transform = transform();
    return transformed_contains(object_bounds, object_transform, pointer);
  }

  void TextWidget::render(RenderRecorder& recorder)
  {
    Text& text = current_text();
    const RenderGeometry geometry = text.geometry();

    if (geometry.count == 0) {
      return;
    }

    recorder.update_text_effect(text.effects());

    RenderObject object = {};
    object.priority = priority();
    object.geometry = geometry;
    object.transform = compute_matrix(text.bounds());
    recorder.record(object);
  }

  RectF TextWidget::bounds() const
  {
    return m_disabled_text.bounds().extend_to(m_default_text.bounds()).extend_to(m_disabled_text.bounds());
  }

  Text& TextWidget::current_text()
  {
    switch (state()) {
      case WidgetState::Disabled:
        return m_disabled_text;
      case WidgetState::Default:
        return m_default_text;
      case WidgetState::Selected:
        return m_selected_text;
    }

    assert(false);
    return m_default_text;
  }

}
