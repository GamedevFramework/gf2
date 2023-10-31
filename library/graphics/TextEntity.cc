// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/TextEntity.h>
// clang-format on

#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  TextEntity::TextEntity(FontAtlas* atlas, const TextData& data, RenderManager* render_manager)
  : m_text(atlas, data, render_manager)
  {
  }

  void TextEntity::render(RenderRecorder& recorder)
  {
    auto text_geometry = m_text.geometry();
    auto model_matrix = m_transform.compute_matrix(m_text.bounds());

    if (text_geometry.outline.has_value()) {
      RenderObject object = {};
      object.priority = priority();
      object.geometry = text_geometry.outline.value();
      object.transform = model_matrix;
      recorder.record(object);
    }

    RenderObject object = {};
    object.priority = priority();
    object.geometry = text_geometry.text;
    object.transform = model_matrix;
    recorder.record(object);
  }

}
