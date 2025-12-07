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
    ConsoleGraphics(ConsoleFont* font);
    ConsoleGraphics(const ConsoleResource& resource, ResourceManager* resource_manager);

    ConsoleFont* font()
    {
      return m_font;
    }

    void set_font(ConsoleFont* font)
    {
      m_font = font;
    }

    void update(const Console& buffer, RenderManager* render_manager);

    RenderGeometry background_geometry();
    RenderGeometry foreground_geometry();

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    void update_background(const Console& buffer, RenderManager* render_manager);
    void update_foreground(const Console& buffer, RenderManager* render_manager);

    ConsoleFont* m_font = nullptr;
    DynamicBuffer m_background_vertices;
    DynamicBuffer m_background_indices;
    DynamicBuffer m_foreground_vertices;
    DynamicBuffer m_foreground_indices;
    RectF m_bounds = {};
  };

}

#endif // GF_CONSOLE_GRAPHICS_H
