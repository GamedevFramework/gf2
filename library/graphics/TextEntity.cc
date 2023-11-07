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

  TextEntity::TextEntity(const TextResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : m_text(resource, render_manager, resource_manager)
  {
  }

  void TextEntity::render(RenderRecorder& recorder)
  {
    auto text_transform = transform();
    text_transform.scale *= m_text.characater_size() / 64.0f;

    RenderObject object = {};
    object.priority = priority();
    object.geometry = m_text.geometry();
    object.transform = text_transform.compute_matrix(m_text.bounds());
    recorder.record(object);
  }

}
