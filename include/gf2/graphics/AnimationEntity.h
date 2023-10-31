// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_ANIMATION_ENTITY_H
#define GF_ANIMATION_ENTITY_H

#include <gf2/core/AnimationData.h>
#include <gf2/core/Transform.h>

#include "Animation.h"
#include "Entity.h"
#include "GraphicsApi.h"
#include "Texture.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API AnimationEntity : public Entity {
  public:
    AnimationEntity(std::vector<const Texture*> textures, const AnimationData& data, RenderManager* render_manager);

    Transform& transform()
    {
      return m_transform;
    }

    const Transform& transform() const
    {
      return m_transform;
    }

    void update(Time time) override;
    void render(RenderRecorder& recorder) override;

    void reset()
    {
      m_sprite.reset();
    }

    bool finished() const
    {
      return m_sprite.finished();
    }

  private:
    Animation m_sprite;
    Transform m_transform;
  };

}

#endif // GF_ANIMATION_ENTITY_H
