// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_ANIMATION_GROUP_H
#define GF_ANIMATION_GROUP_H

#include <string_view>
#include <vector>

#include <gf2/core/AnimationData.h>
#include <gf2/core/Id.h>
#include <gf2/core/Time.h>

#include "AnimationRuntime.h"
#include "Buffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class Texture;
  class RenderManager;
  class ResourceManager;

  class GF_GRAPHICS_API AnimationGroupState {
  public:
    AnimationGroupState(const AnimationGroupData& data);

    void select(std::string_view animation_name);
    void select(Id animation_id);

    void update(Time time);
    void reset();
    bool finished() const;

    Id current_animation_id() const
    {
      return m_current_animation_id;
    }

    std::size_t current_frame() const
    {
      return m_current_frame;
    }

  private:
    const details::AnimationStateRuntime* current_animation() const;

    std::map<Id, details::AnimationStateRuntime> m_animations;
    Id m_current_animation_id = InvalidId;
    std::size_t m_current_frame = 0;
    Time m_current_time;
  };

  class GF_GRAPHICS_API AnimationGroupGraphics {
  public:
    AnimationGroupGraphics(std::vector<const Texture*> textures, const AnimationGroupData& data, RenderManager* render_manager);
    AnimationGroupGraphics(const AnimationGroupResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    RenderGeometry geometry(Id animation_id, std::size_t frame_index) const;

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    std::vector<const Texture*> m_textures;
    Buffer m_vertices;
    Buffer m_indices;
    RectF m_bounds = RectF::from_size({ 0.0f, 0.0f });

    std::map<Id, details::AnimationGraphicsRuntime> m_animations;
  };

  class GF_GRAPHICS_API AnimationGroup {
  public:
    AnimationGroup(std::vector<const Texture*> textures, const AnimationGroupData& data, RenderManager* render_manager);
    AnimationGroup(const AnimationGroupResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    void select(std::string_view animation_name)
    {
      m_state.select(animation_name);
    }

    void select(Id animation_id)
    {
      m_state.select(animation_id);
    }

    void update(Time time)
    {
      m_state.update(time);
    }

    void reset()
    {
      m_state.reset();
    }

    bool finished() const
    {
      return m_state.finished();
    }

    RenderGeometry geometry() const
    {
      return m_graphics.geometry(m_state.current_animation_id(), m_state.current_frame());
    }

    RectF bounds() const
    {
      return m_graphics.bounds();
    }

  private:
    AnimationGroupState m_state;
    AnimationGroupGraphics m_graphics;
  };

}

#endif // GF_ANIMATION_GROUP_H
