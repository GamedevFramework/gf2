// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_ANIMATION_ENTITY_H
#define GF_ANIMATION_ENTITY_H

#include "Animation.h"
#include "GraphicsApi.h"
#include "TransformableEntity.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API AnimationEntity : public TransformableEntity {
  public:
    AnimationEntity(std::vector<const Texture*> textures, const AnimationData& data, RenderManager* render_manager);
    AnimationEntity(const AnimationResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    void update(Time time) override;
    void render(RenderRecorder& recorder) override;

    void reset()
    {
      m_animation.reset();
    }

    bool finished() const
    {
      return m_animation.finished();
    }

  private:
    Animation m_animation;
  };

}

#endif // GF_ANIMATION_ENTITY_H
