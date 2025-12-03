// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_NINE_PATCH_H
#define GF_NINE_PATCH_H

#include <gf2/core/Color.h>
#include <gf2/core/NinePatchData.h>
#include <gf2/core/Rect.h>
#include <gf2/core/Transform.h>

#include "GpuBuffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class GpuTexture;
  class RenderManager;
  class ResourceManager;

  class GF_GRAPHICS_API NinePatch {
  public:
    NinePatch(const GpuTexture* texture, Vec2F size, NinePatchLimits limits, RectF texture_region, Color color, RenderManager* render_manager);
    NinePatch(const GpuTexture* texture, Vec2F size, NinePatchLimits limits, RectF texture_region, RenderManager* render_manager);
    NinePatch(const GpuTexture* texture, Vec2F size, NinePatchLimits limits, RenderManager* render_manager);
    NinePatch(const GpuTexture* texture, const NinePatchData& data, RenderManager* render_manager);
    NinePatch(const NinePatchResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    RenderGeometry geometry() const;

    const GpuTexture* texture() const
    {
      return m_texture;
    }

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    const GpuTexture* m_texture;
    GpuBuffer m_vertices;
    GpuBuffer m_indices;
    RectF m_bounds = {};
  };

}

#endif // GF_NINE_PATCH_H
