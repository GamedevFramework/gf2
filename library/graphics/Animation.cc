// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Animation.h>
// clang-format on

#include <gf2/core/ResourceManager.h>

#include <gf2/graphics/Texture.h>
#include <gf2/graphics/Vertex.h>

#include "gf2/core/Id.h"

namespace gf {

  namespace {

    constexpr std::size_t IndicesPerAnimationFrame = 6;

    template<typename Resource>
    std::vector<const Texture*> load_resources(const Resource& resource, ResourceManager* resource_manager)
    {
      std::vector<const Texture*> resources;
      resources.reserve(resource.textures.size());

      for (const auto& texture : resource.textures) {
        resources.push_back(resource_manager->get<Texture>(texture));
      }

      return resources;
    }

    struct AnimationBuilder {

      std::vector<AnimationFrameRuntime> append_data(const std::vector<const Texture*>& textures, const AnimationData& data)
      {
        std::vector<AnimationFrameRuntime> frames;

        for (const auto& frame_data : data.frames) {
          assert(frame_data.texture_index < textures.size());

          AnimationFrameRuntime frame = {};
          frame.texture_index = frame_data.texture_index;
          frame.duration = frame_data.duration;
          frame.offset = indices.size();
          frames.push_back(frame);

          assert(vertices.size() < UINT16_MAX);
          auto index = static_cast<uint16_t>(vertices.size());

          const Vec2F size = textures[frame_data.texture_index]->size() * frame_data.texture_region.size();
          const RectF bounds = RectF::from_size(size);

          vertices.push_back({ bounds.position_at(Orientation::NorthEast), frame_data.texture_region.position_at(Orientation::NorthEast), data.color });
          vertices.push_back({ bounds.position_at(Orientation::SouthEast), frame_data.texture_region.position_at(Orientation::SouthEast), data.color });
          vertices.push_back({ bounds.position_at(Orientation::NorthWest), frame_data.texture_region.position_at(Orientation::NorthWest), data.color });
          vertices.push_back({ bounds.position_at(Orientation::SouthWest), frame_data.texture_region.position_at(Orientation::SouthWest), data.color });

          // first triangle
          indices.push_back(index);
          indices.push_back(index + 1);
          indices.push_back(index + 2);

          // second triangle
          indices.push_back(index + 2);
          indices.push_back(index + 1);
          indices.push_back(index + 3);

          global_bounds.extend_to(bounds);
        }

        return frames;
      }

      Buffer create_vertices(RenderManager* render_manager)
      {
        return { BufferType::Device, BufferUsage::Vertex, vertices.data(), vertices.size(), render_manager };
      }

      Buffer create_indices(RenderManager* render_manager)
      {
        return { BufferType::Device, BufferUsage::Index, indices.data(), indices.size(), render_manager };
      }

      RectF global_bounds = RectF::from_size({ 0.0f, 0.0f });
      std::vector<Vertex> vertices;
      std::vector<uint16_t> indices;
    };

  }

  /*
   * Animation
   */

  Animation::Animation(std::vector<const Texture*> textures, const AnimationData& data, RenderManager* render_manager)
  : m_textures(std::move(textures))
  , m_animation({ data.properties, {} })
  {
    AnimationBuilder builder;

    m_animation.frames = builder.append_data(m_textures, data);
    m_bounds = builder.global_bounds;

    if (m_animation.frames.empty()) {
      return;
    }

    m_vertices = builder.create_vertices(render_manager);
    m_indices = builder.create_indices(render_manager);
    reset();
  }

  Animation::Animation(const AnimationResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : Animation(load_resources(resource, resource_manager), resource.data, render_manager)
  {
  }

  void Animation::update(Time time)
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
      m_current_time += m_animation.frames[m_current_frame].duration;
    }
  }

  void Animation::reset()
  {
    m_current_frame = 0;

    if (m_animation.frames.empty()) {
      m_current_time = Time();
    } else {
      m_current_time = m_animation.frames.front().duration;
    }
  }

  bool Animation::finished() const
  {
    return !m_animation.properties.test(AnimationProperties::Loop) && m_current_time <= Time() && m_current_frame + 1 >= m_animation.frames.size();
  }

  RenderGeometry Animation::geometry() const
  {
    assert(!m_animation.frames.empty());

    const auto& frame = m_animation.frames[m_current_frame];

    assert(frame.offset == m_current_frame * IndicesPerAnimationFrame);

    RenderGeometry geometry;
    geometry.vertices = &m_vertices;
    geometry.indices = &m_indices;
    geometry.size = IndicesPerAnimationFrame;
    geometry.first = frame.offset;
    geometry.texture = m_textures[frame.texture_index];
    return geometry;
  }

  /*
   * AnimationGroup
   */

  AnimationGroup::AnimationGroup(std::vector<const Texture*> textures, const AnimationGroupData& data, RenderManager* render_manager)
  : m_textures(std::move(textures))
  {
    AnimationBuilder builder;

    for (const auto& [id, animation_data] : data.animations) {
      AnimationRuntime animation;
      animation.properties = animation_data.properties;
      animation.frames = builder.append_data(m_textures, animation_data);
      m_animations.emplace(id, std::move(animation));
    }

    m_bounds = builder.global_bounds;

    m_vertices = builder.create_vertices(render_manager);
    m_indices = builder.create_indices(render_manager);
  }

  AnimationGroup::AnimationGroup(const AnimationGroupResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : AnimationGroup(load_resources(resource, resource_manager), resource.data, render_manager)
  {
  }

  void AnimationGroup::select(std::string_view animation_name)
  {
    select(gf::hash(animation_name));
  }

  void AnimationGroup::select(Id animation_id)
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

  void AnimationGroup::update(Time time)
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
      m_current_time += animation->frames[m_current_frame].duration;
    }
  }

  void AnimationGroup::reset()
  {
    m_current_frame = 0;

    const auto* animation = current_animation();

    if (animation->frames.empty()) {
      m_current_time = Time();
    } else {
      m_current_time = animation->frames.front().duration;
    }
  }

  bool AnimationGroup::finished() const
  {
    const auto* animation = current_animation();
    return !animation->properties.test(AnimationProperties::Loop) && m_current_time <= Time() && m_current_frame + 1 >= animation->frames.size();
  }

  RenderGeometry AnimationGroup::geometry() const
  {
    const auto* animation = current_animation();

    assert(!animation->frames.empty());

    const auto& frame = animation->frames[m_current_frame];

    RenderGeometry geometry;
    geometry.vertices = &m_vertices;
    geometry.indices = &m_indices;
    geometry.size = IndicesPerAnimationFrame;
    geometry.first = frame.offset;
    geometry.texture = m_textures[frame.texture_index];
    return geometry;
  }

  const AnimationRuntime* AnimationGroup::current_animation() const
  {
    if (m_current_animation_id == InvalidId) {
      Log::fatal("No animation selected.");
    }

    if (auto iterator = m_animations.find(m_current_animation_id); iterator != m_animations.end()) {
      return &iterator->second;
    }

    Log::fatal("Unknown selected animation.");
  }

}
