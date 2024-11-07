// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/AnimationGroup.h>

#include <gf2/core/Log.h>

#include "gf2/graphics/AnimationRuntime.h"

namespace gf {

  /*
   * AnimationGroupState
   */

  AnimationGroupState::AnimationGroupState(const AnimationGroupData& data)
  {
    for (const auto& [animation_id, animation_data] : data.animations) {
      details::AnimationStateRuntime runtime;
      runtime.properties = animation_data.properties;

      for (const auto& frame_data : animation_data.frames) {
        runtime.frames.push_back(frame_data.duration);
      }

      m_animations.emplace(animation_id, std::move(runtime));
    }
  }

  void AnimationGroupState::select(std::string_view animation_name)
  {
    select(gf::hash_string(animation_name));
  }

  void AnimationGroupState::select(Id animation_id)
  {
    if (m_current_animation_id == animation_id) {
      return;
    }

    if (auto iterator = m_animations.find(animation_id); iterator == m_animations.end()) {
      Log::fatal("Unknown selected animation.");
    }

    m_current_animation_id = animation_id;
    reset();
  }

  void AnimationGroupState::update(Time time)
  {
    const auto* animation = current_animation();

    if (animation->frames.empty()) {
      return;
    }

    m_current_time -= time;

    while (m_current_time < Time()) {
      if (m_current_frame + 1 >= animation->frames.size() && !animation->properties.test(AnimationProperties::Loop)) {
        return;
      }

      m_current_frame = (m_current_frame + 1) % animation->frames.size();
      m_current_time += animation->frames[m_current_frame];
    }
  }

  void AnimationGroupState::reset()
  {
    m_current_frame = 0;

    const auto* animation = current_animation();

    if (animation->frames.empty()) {
      m_current_time = Time();
    } else {
      m_current_time = animation->frames.front();
    }
  }

  bool AnimationGroupState::finished() const
  {
    const auto* animation = current_animation();
    return !animation->properties.test(AnimationProperties::Loop) && m_current_time <= Time() && m_current_frame + 1 >= animation->frames.size();
  }

  const details::AnimationStateRuntime* AnimationGroupState::current_animation() const
  {
    if (m_current_animation_id == InvalidId) {
      Log::fatal("No animation selected.");
    }

    if (auto iterator = m_animations.find(m_current_animation_id); iterator != m_animations.end()) {
      return &iterator->second;
    }

    Log::fatal("Unknown selected animation.");
  }

  /*
   * AnimationGroupGraphics
   */

  AnimationGroupGraphics::AnimationGroupGraphics(std::vector<const Texture*> textures, const AnimationGroupData& data, RenderManager* render_manager)
  : m_textures(std::move(textures))
  {
    details::AnimationBuilder builder;

    for (const auto& [animation_id, animation_data] : data.animations) {
      m_animations.emplace(animation_id, builder.append_data(m_textures, animation_data));
    }

    m_bounds = builder.global_bounds;

    m_vertices = builder.create_vertices(render_manager);
    m_indices = builder.create_indices(render_manager);
  }

  AnimationGroupGraphics::AnimationGroupGraphics(const AnimationGroupResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : AnimationGroupGraphics(details::load_animation_resources(resource, resource_manager), resource.data, render_manager)
  {
  }

  RenderGeometry AnimationGroupGraphics::geometry(Id animation_id, std::size_t frame_index) const
  {
    auto iterator = m_animations.find(animation_id);

    if (iterator == m_animations.end()) {
      return {};
    }

    const auto& animation = iterator->second;

    assert(frame_index < animation.frames.size());

    const auto& frame = animation.frames[frame_index];

    RenderGeometry geometry;
    geometry.vertices = &m_vertices;
    geometry.indices = &m_indices;
    geometry.size = details::IndicesPerAnimationFrame;
    geometry.first = frame.offset;
    geometry.texture = m_textures[frame.texture_index];
    return geometry;
  }

  /*
   * AnimationGroup
   */

  AnimationGroup::AnimationGroup(std::vector<const Texture*> textures, const AnimationGroupData& data, RenderManager* render_manager)
  : m_state(data)
  , m_graphics(std::move(textures), data, render_manager)
  {
  }

  AnimationGroup::AnimationGroup(const AnimationGroupResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : m_state(resource.data)
  , m_graphics(resource, render_manager, resource_manager)
  {
  }

}
