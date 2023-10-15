// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/AnimationEntity.h>
// clang-format on

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  AnimationEntity::AnimationEntity(std::vector<const Texture*> textures, const AnimationData& data, Renderer* renderer)
  : m_sprite(std::move(textures), data, renderer)
  {
  }

  void AnimationEntity::update(Time time)
  {
    m_sprite.update(time);
  }

  void AnimationEntity::render(RenderRecorder& recorder)
  {
    RenderObject object = {};
    object.priority = priority();
    object.geometry = m_sprite.geometry();
    object.transform = m_transform.compute_matrix(m_sprite.bounds());
    recorder.record(object);
  }

}
