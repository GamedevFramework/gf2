// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SPRITE_ENTITY_H
#define GF_SPRITE_ENTITY_H

#include <gf2/core/Transform.h>

#include "GraphicsApi.h"
#include "Sprite.h"
#include "GpuTexture.h"
#include "TransformableEntity.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API SpriteEntity : public TransformableEntity {
  public:
    SpriteEntity(GpuTexture* texture, RenderManager* render_manager);
    SpriteEntity(GpuTexture* texture, const SpriteData& data, RenderManager* render_manager);
    SpriteEntity(const SpriteResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    Sprite& sprite()
    {
      return m_sprite;
    }

    void render(RenderRecorder& recorder) override;

  private:
    Sprite m_sprite;
  };

}

#endif // GF_SPRITE_ENTITY_H
