// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/AnimationEntity.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  /*
   * AnimationEntity
   */

  AnimationEntity::AnimationEntity(std::vector<const Texture*> textures, const AnimationData& data, RenderManager* render_manager)
  : m_animation(std::move(textures), data, render_manager)
  {
  }

  AnimationEntity::AnimationEntity(const AnimationResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : m_animation(resource, render_manager, resource_manager)
  {
  }

  void AnimationEntity::update(Time time)
  {
    m_animation.update(time);
  }

  void AnimationEntity::render(RenderRecorder& recorder)
  {
    RenderObject object = {};
    object.priority = priority();
    object.geometry = m_animation.geometry();
    object.transform = compute_matrix(m_animation.bounds());
    recorder.record(object);
  }

}
