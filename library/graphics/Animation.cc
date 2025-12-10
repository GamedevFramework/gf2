// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Animation.h>

#include <gf2/core/ResourceManager.h>

#include <gf2/graphics/GpuTexture.h>
#include <gf2/graphics/Vertex.h>

namespace gf {

  /*
   * AnimationState
   */

  AnimationState::AnimationState(const AnimationData& data)
  : m_animation({ data.properties, {} })
  {
    m_animation.properties = data.properties;

    for (const auto& frame_data : data.frames) {
      m_animation.frames.push_back(frame_data.duration);
    }

    reset();
  }

  void AnimationState::update(Time time)
  {
    if (m_animation.frames.empty()) {
      return;
    }

    m_current_time -= time;

    while (m_current_time < Time()) {
      if (m_current_frame + 1 >= m_animation.frames.size() && !m_animation.properties.test(AnimationProperties::Loop)) {
        return;
      }

      m_current_frame = (m_current_frame + 1) % m_animation.frames.size();
      m_current_time += m_animation.frames[m_current_frame];
    }
  }

  void AnimationState::reset()
  {
    m_current_frame = 0;

    if (m_animation.frames.empty()) {
      m_current_time = Time();
    } else {
      m_current_time = m_animation.frames.front();
    }
  }

  bool AnimationState::finished() const
  {
    return !m_animation.properties.test(AnimationProperties::Loop) && m_current_time <= Time() && m_current_frame + 1 >= m_animation.frames.size();
  }

  /*
   * AnimationGraphics
   */

  AnimationGraphics::AnimationGraphics(std::vector<GpuTexture*> textures, const AnimationData& data, RenderManager* render_manager)
  : m_textures(std::move(textures))
  {
    details::AnimationBuilder builder;

    m_animation = builder.append_data(m_textures, data);
    m_bounds = builder.global_bounds;

    if (m_animation.frames.empty()) {
      return;
    }

    m_vertices = builder.create_vertices(render_manager);
    m_indices = builder.create_indices(render_manager);
  }

  AnimationGraphics::AnimationGraphics(const AnimationResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : AnimationGraphics(details::load_animation_resources(resource, resource_manager), resource.data, render_manager)
  {
  }

  RenderGeometry AnimationGraphics::geometry(std::size_t frame_index)
  {
    assert(frame_index < m_animation.frames.size());

    const auto& frame = m_animation.frames[frame_index];

    assert(frame.offset == frame_index * details::IndicesPerAnimationFrame);

    RenderGeometry geometry;
    geometry.vertices = &m_vertices;
    geometry.indices = &m_indices;
    geometry.count = details::IndicesPerAnimationFrame;
    geometry.first = frame.offset;
    geometry.texture = m_textures[frame.texture_index];
    return geometry;
  }

  /*
   * Animation
   */

  Animation::Animation(std::vector<GpuTexture*> textures, const AnimationData& data, RenderManager* render_manager)
  : m_state(data)
  , m_graphics(std::move(textures), data, render_manager)
  {
  }

  Animation::Animation(const AnimationResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : m_state(resource.data)
  , m_graphics(resource, render_manager, resource_manager)
  {
  }

}
