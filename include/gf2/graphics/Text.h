// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TEXT_H
#define GF_TEXT_H

#include <gf2/core/Rect.h>
#include <gf2/core/TextData.h>

#include "Buffer.h"
#include "FontAtlas.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class RenderManager;
  class ResourceManager;

  class GF_GRAPHICS_API Text {
  public:
    Text(FontAtlas* atlas, const TextData& data, RenderManager* render_manager);
    Text(const TextResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    RenderGeometry geometry() const;

    const Texture* texture() const
    {
      return m_atlas->texture();
    }

    const Buffer* vertices() const
    {
      return &m_vertices;
    }

    const Buffer* indices() const
    {
      return &m_indices;
    }

    std::size_t index_count() const
    {
      return m_indices.count();
    }

    RectF bounds() const
    {
      return m_bounds;
    }

    void set_characater_size(float character_size);
    float characater_size() const;

  private:
    FontAtlas* m_atlas;
    Buffer m_vertices;
    Buffer m_indices;
    RectF m_bounds = {};
    float m_character_size;
  };

}

#endif // GF_TEXT_H
