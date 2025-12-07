// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_ENTITY_H
#define GF_CONSOLE_ENTITY_H

#include "ConsoleGraphics.h"
#include "GraphicsApi.h"
#include "TransformableEntity.h"

namespace gf {

  class GF_GRAPHICS_API ConsoleEntity : public TransformableEntity {
  public:
    ConsoleEntity() = default;
    ConsoleEntity(ConsoleFont* font);
    ConsoleEntity(ConsoleResource& resource, ResourceManager* resource_manager);

    ConsoleGraphics& graphics()
    {
      return m_console;
    }

    void render(RenderRecorder& recorder) override;

  private:
    ConsoleGraphics m_console;
  };

}

#endif // GF_CONSOLE_ENTITY_H
