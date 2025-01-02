// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_NINE_PATCH_H
#define GF_NINE_PATCH_H

#include <gf2/core/Color.h>
#include <gf2/core/NinePatchData.h>
#include <gf2/core/Rect.h>
#include <gf2/core/Transform.h>

#include "Buffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class RenderManager;
  class ResourceManager;
  class Texture;

  class GF_GRAPHICS_API NinePatch {
  public:
    NinePatch(const Texture* texture, Vec2F size, NinePatchLimits limits, RectF texture_region, Color color, RenderManager* render_manager);
    NinePatch(const Texture* texture, Vec2F size, NinePatchLimits limits, RectF texture_region, RenderManager* render_manager);
    NinePatch(const Texture* texture, Vec2F size, NinePatchLimits limits, RenderManager* render_manager);
    NinePatch(const Texture* texture, const NinePatchData& data, RenderManager* render_manager);
    NinePatch(const NinePatchResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    RenderGeometry geometry() const;

    const Texture* texture() const
    {
      return m_texture;
    }

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    const Texture* m_texture;
    Buffer m_vertices;
    Buffer m_indices;
    RectF m_bounds = {};
  };

}

#endif // GF_NINE_PATCH_H
