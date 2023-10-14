// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_ANIMATED_SPRITE_ENTITY_H
#define GF_ANIMATED_SPRITE_ENTITY_H

#include <gf2/core/AnimationData.h>
#include <gf2/core/Transform.h>

#include "AnimatedSprite.h"
#include "Entity.h"
#include "GraphicsApi.h"
#include "Texture.h"

namespace gf {
  class Renderer;

  class GF_GRAPHICS_API AnimatedSpriteEntity : public Entity {
  public:
    AnimatedSpriteEntity(std::vector<const Texture*> textures, const AnimationData& data, Renderer* renderer);

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
    AnimatedSprite m_sprite;
    Transform m_transform;
  };

}

#endif // GF_ANIMATED_SPRITE_ENTITY_H
