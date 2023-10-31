// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_ANIMATION_H
#define GF_ANIMATION_H

#include <vector>

#include <gf2/core/AnimationData.h>
#include <gf2/core/Flags.h>
#include <gf2/core/Rect.h>
#include <gf2/core/Time.h>

#include "Buffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class Texture;
  class RenderManager;

  class GF_GRAPHICS_API Animation {
  public:
    Animation(std::vector<const Texture*> textures, const AnimationData& data, RenderManager* render_manager);

    void update(Time time);
    void reset();
    bool finished() const;

    RenderGeometry geometry() const;

    const Texture* texture() const;
    const Buffer* vertices() const;
    const Buffer* indices() const;
    std::size_t index_count() const;
    std::size_t first_index() const;

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    std::vector<const Texture*> m_textures;
    Buffer m_vertices;
    Buffer m_indices;
    RectF m_bounds = RectF::from_size({ 0.0f, 0.0f });

    struct AnimationFrame {
      std::size_t texture_index = 0;
      Time duration = {};
    };

    Flags<AnimationProperties> m_properties = None;
    std::vector<AnimationFrame> m_frames;
    std::size_t m_current_frame = 0;
    Time m_current_time = {};
  };

}

#endif // GF_ANIMATION_H
