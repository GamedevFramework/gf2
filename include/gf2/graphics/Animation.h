// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_ANIMATION_H
#define GF_ANIMATION_H

#include <string_view>
#include <vector>

#include <gf2/core/AnimationData.h>
#include <gf2/core/Flags.h>
#include <gf2/core/Rect.h>
#include <gf2/core/Time.h>

#include "Buffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

#include "gf2/core/Id.h"

namespace gf {
  class Texture;
  class RenderManager;
  class ResourceManager;

  struct GF_GRAPHICS_API AnimationFrameRuntime {
    std::size_t texture_index = 0;
    Time duration = {};
    std::size_t offset = 0;
  };

  struct GF_GRAPHICS_API AnimationRuntime {
    Flags<AnimationProperties> properties = None;
    std::vector<AnimationFrameRuntime> frames;
  };

  class GF_GRAPHICS_API Animation {
  public:
    Animation(std::vector<const Texture*> textures, const AnimationData& data, RenderManager* render_manager);
    Animation(const AnimationResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    void update(Time time);
    void reset();
    bool finished() const;

    RenderGeometry geometry() const;

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    std::vector<const Texture*> m_textures;
    Buffer m_vertices;
    Buffer m_indices;
    RectF m_bounds = RectF::from_size({ 0.0f, 0.0f });

    AnimationRuntime m_animation;
    std::size_t m_current_frame = 0;
    Time m_current_time = {};
  };

  class GF_GRAPHICS_API AnimationGroup {
  public:
    AnimationGroup(std::vector<const Texture*> textures, const AnimationGroupData& data, RenderManager* render_manager);
    AnimationGroup(const AnimationGroupResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    void select(std::string_view animation_name);
    void select(Id animation_id);

    void update(Time time);
    void reset();
    bool finished() const;

    RenderGeometry geometry() const;

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    const AnimationRuntime* current_animation() const;

    std::vector<const Texture*> m_textures;
    Buffer m_vertices;
    Buffer m_indices;
    RectF m_bounds = RectF::from_size({ 0.0f, 0.0f });

    std::map<Id, AnimationRuntime> m_animations;
    Id m_current_animation_id = InvalidId;
    std::size_t m_current_frame = 0;
    Time m_current_time = {};
  };
}

#endif // GF_ANIMATION_H
