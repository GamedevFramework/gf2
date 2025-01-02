// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/AnimationGroupEntity.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  /*
   * AnimationGroupEntity
   */

  AnimationGroupEntity::AnimationGroupEntity(std::vector<const Texture*> textures, const AnimationGroupData& data, RenderManager* render_manager)
  : m_animation_group(std::move(textures), data, render_manager)
  {
  }

  AnimationGroupEntity::AnimationGroupEntity(const AnimationGroupResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : m_animation_group(resource, render_manager, resource_manager)
  {
  }

  void AnimationGroupEntity::update(Time time)
  {
    m_animation_group.update(time);
  }

  void AnimationGroupEntity::render(RenderRecorder& recorder)
  {
    RenderObject object = {};
    object.priority = priority();
    object.geometry = m_animation_group.geometry();
    object.transform = compute_matrix(m_animation_group.bounds());
    recorder.record(object);
  }

}
