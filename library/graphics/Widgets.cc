// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Widgets.h>

#include <gf2/graphics/RenderRecorder.h>
#include "gf2/graphics/Sprite.h"

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
    render_text(recorder);
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

  void TextWidget::render_text(RenderRecorder& recorder)
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
    object.transform = compute_matrix(bounds());
    recorder.record(object);
  }

  /*
   * TextButtonWidget
   */

  namespace {

    ShapeBuffer make_basic_shape_buffer(const ButtonWidgetData& data, RectF bounds)
    {
      const RectF shape_bounds = bounds.grow_by(data.border_padding);

      if (data.corner_radius == 0.0f) {
        return ShapeBuffer::make_rectangle(shape_bounds);
      }

      return ShapeBuffer::make_rounded_rectangle(shape_bounds, data.corner_radius);
    }


    ShapeBuffer make_shape_buffer(const ButtonWidgetData& data, RectF bounds)
    {
      ShapeBuffer buffer = make_basic_shape_buffer(data, bounds);
      buffer.color = data.color;
      buffer.outline_thickness = data.outline_thickness;
      buffer.outline_color = data.outline_color;
      return buffer;
    }

  }

  TextButtonWidget::TextButtonWidget(FontAtlas* atlas, FontFace* face, const TextButtonWidgetData& data, RenderManager* render_manager)
  : TextWidget(atlas, face, data, render_manager)
  , m_disabled_shape(nullptr, make_shape_buffer(data.disabled_button, TextWidget::bounds()), render_manager)
  , m_default_shape(nullptr, make_shape_buffer(data.default_button, TextWidget::bounds()), render_manager)
  , m_selected_shape(nullptr, make_shape_buffer(data.selected_button, TextWidget::bounds()), render_manager)
  {
  }

  TextButtonWidget::TextButtonWidget(FontAtlas* atlas, const TextButtonWidgetResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : TextWidget(atlas, { resource.font, static_cast<const TextWidgetData&>(resource.data) }, render_manager, resource_manager )
  , m_disabled_shape({ "", make_shape_buffer(resource.data.disabled_button, TextWidget::bounds()) }, render_manager, resource_manager)
  , m_default_shape({ "", make_shape_buffer(resource.data.disabled_button, TextWidget::bounds()) }, render_manager, resource_manager)
  , m_selected_shape({ "", make_shape_buffer(resource.data.disabled_button, TextWidget::bounds()) }, render_manager, resource_manager)
  {
  }

  bool TextButtonWidget::contains(Vec2F pointer)
  {
    const RectF object_bounds = bounds();
    const Transform object_transform = transform();
    return transformed_contains(object_bounds, object_transform, pointer);
  }

  void TextButtonWidget::render(RenderRecorder& recorder)
  {
    render_shape(recorder);
    render_text(recorder);
  }

  RectF TextButtonWidget::bounds() const
  {
    return m_disabled_shape.bounds().extend_to(m_default_shape.bounds()).extend_to(m_selected_shape.bounds());
  }

  Shape& TextButtonWidget::current_shape()
  {
    switch (state()) {
      case WidgetState::Disabled:
        return m_disabled_shape;
      case WidgetState::Default:
        return m_default_shape;
      case WidgetState::Selected:
        return m_selected_shape;
    }

    assert(false);
    return m_default_shape;
  }

  void TextButtonWidget::render_shape(RenderRecorder& recorder)
  {
    Shape& shape = current_shape();
    const ShapeGeometry geometry = shape.geometry();

    if (geometry.shape.count == 0) {
      return;
    }

    RenderObject object = {};
    object.priority = priority();
    object.transform = compute_matrix(bounds());

    if (geometry.outline) {
      object.geometry = geometry.outline.value();
      recorder.record(object);
    }

    object.geometry = geometry.shape;
    recorder.record(object);
  }

  /*
   * SpriteWidget
   */

  SpriteWidget::SpriteWidget(GpuTexture* disabled_texture, GpuTexture* default_texture, GpuTexture* selected_texture, const SpriteWidgetData& data, RenderManager* render_manager)
  : m_disabled_sprite(disabled_texture, data.disabled_sprite, render_manager)
  , m_default_sprite(default_texture, data.default_sprite, render_manager)
  , m_selected_sprite(selected_texture, data.selected_sprite, render_manager)
  {
  }

  SpriteWidget::SpriteWidget(const SpriteWidgetResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : m_disabled_sprite({ resource.disabled_texture, resource.data.disabled_sprite }, render_manager, resource_manager)
  , m_default_sprite({ resource.default_texture, resource.data.default_sprite }, render_manager, resource_manager)
  , m_selected_sprite({ resource.selected_texture, resource.data.selected_sprite }, render_manager, resource_manager)
  {
  }

  bool SpriteWidget::contains(Vec2F pointer)
  {
    const RectF object_bounds = bounds();
    const Transform object_transform = transform();
    return transformed_contains(object_bounds, object_transform, pointer);
  }

  void SpriteWidget::render(RenderRecorder& recorder)
  {
    Sprite& sprite = current_sprite();
    const RenderGeometry geometry = sprite.geometry();

    if (geometry.count == 0) {
      return;
    }

    RenderObject object = {};
    object.priority = priority();
    object.geometry = geometry;
    object.transform = compute_matrix(bounds());
    recorder.record(object);
  }

  RectF SpriteWidget::bounds() const
  {
    return m_disabled_sprite.bounds().extend_to(m_default_sprite.bounds()).extend_to(m_selected_sprite.bounds());
  }

  Sprite& SpriteWidget::current_sprite()
  {
    switch (state()) {
      case WidgetState::Disabled:
        return m_disabled_sprite;
      case WidgetState::Default:
        return m_default_sprite;
      case WidgetState::Selected:
        return m_selected_sprite;
    }

    assert(false);
    return m_default_sprite;
  }

}
