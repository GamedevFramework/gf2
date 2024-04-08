// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CONSOLE_H
#define GF_CONSOLE_H

#include <gf2/core/ConsoleData.h>

#include "ConsoleFont.h"
#include "DynamicBuffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class RenderManager;
  class ResourceManager;

  class GF_GRAPHICS_API Console {
  public:
    Console();
    Console(const ConsoleFont* font, const ConsoleData& data, RenderManager* render_manager);
    Console(const ConsoleResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    const ConsoleFont* font() const
    {
      return m_font;
    }

    void set_font(const ConsoleFont* font)
    {
      m_font = font;
    }

    void update(const ConsoleData& data, RenderManager* render_manager);

    RenderGeometry background_geometry() const;
    RenderGeometry foreground_geometry() const;

    RectF bounds() const
    {
      return m_bounds;
    }

  private:
    void update_background(const ConsoleData& data, RenderManager* render_manager);
    void update_foreground(const ConsoleData& data, RenderManager* render_manager);

    const ConsoleFont* m_font = nullptr;
    DynamicBuffer m_background_vertices;
    DynamicBuffer m_background_indices;
    DynamicBuffer m_foreground_vertices;
    DynamicBuffer m_foreground_indices;
    RectF m_bounds = {};
  };


}

#endif // GF_CONSOLE_H
