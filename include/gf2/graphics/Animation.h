// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_ANIMATION_H
#define GF_ANIMATION_H

#include <string_view>
#include <vector>

#include <gf2/core/AnimationData.h>
#include <gf2/core/Flags.h>
#include <gf2/core/Id.h>
#include <gf2/core/Rect.h>
#include <gf2/core/Time.h>

#include "AnimationRuntime.h"
#include "GpuBuffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class GpuTexture;
  class RenderManager;
  class ResourceManager;

  class GF_GRAPHICS_API AnimationState {
  public:
    AnimationState(const AnimationData& data);

    void update(Time time);
    void reset();
    bool finished() const;

    std::size_t current_frame() const
    {
      return m_current_frame;
    }

  private:
    details::AnimationStateRuntime m_animation;
    std::size_t m_current_frame = 0;
    Time m_current_time;
  };

  class GF_GRAPHICS_API AnimationGraphics {
  public:
    AnimationGraphics(std::vector<const GpuTexture*> textures, const AnimationData& data, RenderManager* render_manager);
    AnimationGraphics(const AnimationResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    RenderGeometry geometry(std::size_t frame_index) const;

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    std::vector<const GpuTexture*> m_textures;
    GpuBuffer m_vertices;
    GpuBuffer m_indices;
    RectF m_bounds = RectF::from_size({ 0.0f, 0.0f });
    details::AnimationGraphicsRuntime m_animation;
  };

  class GF_GRAPHICS_API Animation {
  public:
    Animation(std::vector<const GpuTexture*> textures, const AnimationData& data, RenderManager* render_manager);
    Animation(const AnimationResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

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
      return m_graphics.geometry(m_state.current_frame());
    }

    RectF bounds() const
    {
      return m_graphics.bounds();
    }

  private:
    AnimationState m_state;
    AnimationGraphics m_graphics;
  };

}

#endif // GF_ANIMATION_H
