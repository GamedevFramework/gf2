// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/TextEntity.h>

#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  /*
   * Text
   */

  TextEntity::TextEntity(FontAtlas* atlas, FontFace* face, const TextData& data, RenderManager* render_manager)
  : m_text(atlas, face, data, render_manager)
  {
  }

  TextEntity::TextEntity(FontAtlas* atlas, const TextResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : m_text(atlas, resource, render_manager, resource_manager)
  {
  }

  void TextEntity::render(RenderRecorder& recorder)
  {
    recorder.update_text_effect(m_text.effects());

    RenderObject object = {};
    object.priority = priority();
    object.geometry = m_text.geometry();
    object.transform = compute_matrix(m_text.bounds());
    recorder.record(object);
  }

  /*
   * RichText
   */

  RichTextEntity::RichTextEntity(FontAtlas* atlas, RichTextStyle* style, FontFace* default_font, FontFace* bold_font, FontFace* italic_font, FontFace* bold_italic_font, const TextData& data, RenderManager* render_manager)
  : m_text(atlas, style, default_font, bold_font, italic_font, bold_italic_font, data, render_manager)
  {
  }

  RichTextEntity::RichTextEntity(FontAtlas* atlas, RichTextStyle* style, const RichTextResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : m_text(atlas, style, resource, render_manager, resource_manager)
  {
  }

  void RichTextEntity::render(RenderRecorder& recorder)
  {
    recorder.update_text_effect(m_text.effects());

    RenderObject object = {};
    object.priority = priority();
    object.geometry = m_text.geometry();
    object.transform = compute_matrix(m_text.bounds());
    recorder.record(object);
  }

}
