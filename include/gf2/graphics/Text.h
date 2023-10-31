// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TEXT_H
#define GF_TEXT_H

#include <optional>

#include <gf2/core/Rect.h>
#include <gf2/core/TextData.h>

#include "Buffer.h"
#include "FontAtlas.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class RenderManager;

  struct TextGeometry {
    RenderGeometry text;
    std::optional<RenderGeometry> outline;
  };

  class GF_GRAPHICS_API Text {
  public:
    Text(FontAtlas* atlas, const TextData& data, RenderManager* render_manager);

    TextGeometry geometry() const;

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

    bool has_outline() const
    {
      return m_outline_vertices.has_value();
    }

    const Buffer* outline_vertices() const
    {
      return m_outline_vertices.has_value() ? &m_outline_vertices.value() : nullptr;
    }

    const Buffer* outline_indices() const
    {
      return m_outline_indices.has_value() ? &m_outline_indices.value() : nullptr;
    }

    std::size_t outline_index_count() const
    {
      return m_outline_indices.has_value() ? m_outline_indices.value().count() : 0;
    }

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    FontAtlas* m_atlas;
    Buffer m_vertices;
    Buffer m_indices;
    std::optional<Buffer> m_outline_vertices;
    std::optional<Buffer> m_outline_indices;
    RectF m_bounds = {};
  };

}

#endif // GF_TEXT_H
