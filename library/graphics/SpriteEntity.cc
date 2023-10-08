// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/SpriteEntity.h>
// clang-format on

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  SpriteEntity::SpriteEntity(const Texture* texture, Renderer* renderer)
  : m_sprite(texture, renderer)
  {
  }

  void SpriteEntity::render(RenderRecorder& recorder)
  {
    RenderObject object = {};
    object.priority = priority();
    object.geometry = m_sprite.geometry();
    object.transform = m_transform.compute_matrix(m_sprite.bounds());
    recorder.record(object);
  }

}
