// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_GRAPHICS_H
#define GF_CONSOLE_GRAPHICS_H

#include <gf2/core/Console.h>
#include <gf2/core/ConsoleFontResource.h>

#include "ConsoleFont.h"
#include "GpuDynamicBuffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class RenderManager;
  class ResourceManager;

  class GF_GRAPHICS_API ConsoleGraphics {
  public:
    ConsoleGraphics();
    ConsoleGraphics(ConsoleFont* picture_font, ConsoleFont* text_font = nullptr);
    ConsoleGraphics(const ConsoleFontResource& resource, ResourceManager* resource_manager);
    ConsoleGraphics(const MixedConsoleFontResource& resource, ResourceManager* resource_manager);

    ConsoleFont* picture_font()
    {
      return m_picture_font;
    }

    ConsoleFont* text_font()
    {
      return m_text_font;
    }

    void set_picture_font(ConsoleFont* font)
    {
      m_picture_font = font;
    }

    void set_text_font(ConsoleFont* font)
    {
      m_text_font = font;
    }

    void update(const Console& console, RenderManager* render_manager);

    RenderGeometry background_geometry();
    RenderGeometry foreground_picture_geometry();
    RenderGeometry foreground_text_geometry();

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    void update_background(const Console& console, RenderManager* render_manager);
    void update_foreground_picture(const Console& console, RenderManager* render_manager);
    void update_foreground_text(const Console& console, RenderManager* render_manager);

    ConsoleFont* m_picture_font = nullptr;
    ConsoleFont* m_text_font = nullptr;
    GpuDynamicBuffer m_background_vertices;
    GpuDynamicBuffer m_background_indices;
    GpuDynamicBuffer m_foreground_picture_vertices;
    GpuDynamicBuffer m_foreground_picture_indices;
    GpuDynamicBuffer m_foreground_text_vertices;
    GpuDynamicBuffer m_foreground_text_indices;
    RectF m_bounds = {};
  };

}

#endif // GF_CONSOLE_GRAPHICS_H
