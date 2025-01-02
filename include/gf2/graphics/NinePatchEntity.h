// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_NINE_PATCH_ENTITY_H
#define GF_NINE_PATCH_ENTITY_H

#include <gf2/core/Transform.h>

#include "GraphicsApi.h"
#include "NinePatch.h"
#include "Texture.h"
#include "TransformableEntity.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API NinePatchEntity : public TransformableEntity {
  public:
    NinePatchEntity(const Texture* texture, Vec2F size, NinePatchLimits limits, RenderManager* render_manager);
    NinePatchEntity(const Texture* texture, const NinePatchData& data, RenderManager* render_manager);
    NinePatchEntity(const NinePatchResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    const NinePatch& nine_patch() const
    {
      return m_nine_patch;
    }

    void render(RenderRecorder& recorder) override;

  private:
    NinePatch m_nine_patch;
  };

}

#endif // GF_NINE_PATCH_ENTITY_H
