// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/AnimationEntity.h>
// clang-format on

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>
#include "gf2/graphics/Animation.h"

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
