// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/SpriteEntity.h>
// clang-format on

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  SpriteEntity::SpriteEntity(const Texture* texture, RenderManager* render_manager)
  : m_sprite(texture, render_manager)
  {
  }

  SpriteEntity::SpriteEntity(const SpriteResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : m_sprite(resource, render_manager, resource_manager)
  {
  }

  void SpriteEntity::render(RenderRecorder& recorder)
  {
    RenderObject object = {};
    object.priority = priority();
    object.geometry = m_sprite.geometry();
    object.transform = compute_matrix(m_sprite.bounds());
    recorder.record(object);
  }

}
