// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_ANIMATION_GROUP_ENTITY_H
#define GF_ANIMATION_GROUP_ENTITY_H

#include "AnimationGroup.h"
#include "GraphicsApi.h"
#include "TransformableEntity.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API AnimationGroupEntity : public TransformableEntity {
  public:
    AnimationGroupEntity(std::vector<GpuTexture*> textures, const AnimationGroupData& data, RenderManager* render_manager);
    AnimationGroupEntity(const AnimationGroupResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    void update(Time time) override;
    void render(RenderRecorder& recorder) override;

    void select(std::string_view animation_name)
    {
      m_animation_group.select(animation_name);
    }

    void select(Id animation_id)
    {
      m_animation_group.select(animation_id);
    }

    void reset()
    {
      m_animation_group.reset();
    }

    bool finished() const
    {
      return m_animation_group.finished();
    }

  private:
    AnimationGroup m_animation_group;
  };

}

#endif // GF_ANIMATION_ENTITY_H
