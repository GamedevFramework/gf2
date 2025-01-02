// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/NinePatchEntity.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  NinePatchEntity::NinePatchEntity(const Texture* texture, Vec2F size, NinePatchLimits limits, RenderManager* render_manager)
  : m_nine_patch(texture, size, limits, render_manager)
  {
  }

  NinePatchEntity::NinePatchEntity(const Texture* texture, const NinePatchData& data, RenderManager* render_manager)
  : m_nine_patch(texture, data, render_manager)
  {
  }

  NinePatchEntity::NinePatchEntity(const NinePatchResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : m_nine_patch(resource, render_manager, resource_manager)
  {
  }

  void NinePatchEntity::render(RenderRecorder& recorder)
  {
    RenderObject object = {};
    object.priority = priority();
    object.geometry = m_nine_patch.geometry();
    object.transform = compute_matrix(m_nine_patch.bounds());
    recorder.record(object);
  }

}
