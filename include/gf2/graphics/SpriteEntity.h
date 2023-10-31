// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SPRITE_ENTITY_H
#define GF_SPRITE_ENTITY_H

#include <gf2/core/Transform.h>

#include "Entity.h"
#include "GraphicsApi.h"
#include "Sprite.h"
#include "Texture.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API SpriteEntity : public Entity {
  public:
    SpriteEntity(const Texture* texture, RenderManager* render_manager);
    SpriteEntity(const SpriteResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    Transform& transform()
    {
      return m_transform;
    }

    const Transform& transform() const
    {
      return m_transform;
    }

    void render(RenderRecorder& recorder) override;

  private:
    Sprite m_sprite;
    Transform m_transform;
  };

}

#endif // GF_SPRITE_ENTITY_H
