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
    auto geometry = m_text.geometry();
    auto transform = m_transform;
    transform.scale *= m_text.characater_size() / 64.0f;
    auto model_matrix = transform.compute_matrix(m_text.bounds());

    RenderObject object = {};
    object.priority = priority();
    object.geometry = geometry;
    object.transform = model_matrix;
    recorder.record(object);
  }

}
