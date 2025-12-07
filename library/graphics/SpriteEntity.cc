// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/SpriteEntity.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  SpriteEntity::SpriteEntity(GpuTexture* texture, RenderManager* render_manager)
  : m_sprite(texture, render_manager)
  {
  }

  SpriteEntity::SpriteEntity(GpuTexture* texture, const SpriteData& data, RenderManager* render_manager)
  : m_sprite(texture, data, render_manager)
  {
  }

  SpriteEntity::SpriteEntity(const SpriteResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : m_sprite(resource, render_manager, resource_manager)
  {
  }

  void SpriteEntity::render(RenderRecorder& recorder)
  {
    RenderObject object = {};
    object.priority = priority();
    object.geometry = m_sprite.geometry();
    object.transform = compute_matrix(m_sprite.bounds());
    recorder.record(object);
  }

}
