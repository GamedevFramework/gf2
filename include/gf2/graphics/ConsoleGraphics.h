// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_GRAPHICS_H
#define GF_CONSOLE_GRAPHICS_H

#include <gf2/core/Console.h>
#include <gf2/core/ConsoleData.h>

#include "ConsoleFont.h"
#include "DynamicBuffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class RenderManager;
  class ResourceManager;

  class GF_GRAPHICS_API ConsoleGraphics {
  public:
    ConsoleGraphics();
    ConsoleGraphics(const ConsoleFont* font);
    ConsoleGraphics(const ConsoleResource& resource, ResourceManager* resource_manager);

    const ConsoleFont* font() const
    {
      return m_font;
    }

    void set_font(const ConsoleFont* font)
    {
      m_font = font;
    }

    void update(const Console& buffer, RenderManager* render_manager);

    RenderGeometry background_geometry() const;
    RenderGeometry foreground_geometry() const;

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    void update_background(const Console& buffer, RenderManager* render_manager);
    void update_foreground(const Console& buffer, RenderManager* render_manager);

    const ConsoleFont* m_font = nullptr;
    DynamicBuffer m_background_vertices;
    DynamicBuffer m_background_indices;
    DynamicBuffer m_foreground_vertices;
    DynamicBuffer m_foreground_indices;
    RectF m_bounds = {};
  };

}

#endif // GF_CONSOLE_GRAPHICS_H
