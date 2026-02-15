// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_GRAPHICS_ENTITY_H
#define GF_CONSOLE_GRAPHICS_ENTITY_H

#include "ConsoleGraphics.h"
#include "GraphicsApi.h"
#include "TransformableEntity.h"

namespace gf {

  class GF_GRAPHICS_API ConsoleGraphicsEntity : public TransformableEntity {
  public:
    ConsoleGraphicsEntity() = default;
    ConsoleGraphicsEntity(ConsoleFont* picture_font, ConsoleFont* text_font = nullptr);
    ConsoleGraphicsEntity(const ConsoleFontResource& resource, ResourceManager* resource_manager);
    ConsoleGraphicsEntity(const MixedConsoleFontResource& resource, ResourceManager* resource_manager);

    ConsoleGraphics& graphics()
    {
      return m_console;
    }

    void render(RenderRecorder& recorder) override;

  private:
    ConsoleGraphics m_console;
  };

}

#endif // GF_CONSOLE_GRAPHICS_ENTITY_H
