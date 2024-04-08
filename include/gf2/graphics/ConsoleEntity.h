// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CONSOLE_ENTITY_H
#define GF_CONSOLE_ENTITY_H

#include "Console.h"
#include "GraphicsApi.h"
#include "TransformableEntity.h"

namespace gf {

  class GF_GRAPHICS_API ConsoleEntity : public TransformableEntity {
  public:
    ConsoleEntity() = default;
    ConsoleEntity(const ConsoleFont* font, const ConsoleData& data, RenderManager* render_manager);
    ConsoleEntity(const ConsoleResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    Console& console()
    {
      return m_console;
    }

    void render(RenderRecorder& recorder) override;

  private:
    Console m_console;
  };

}

#endif // GF_CONSOLE_ENTITY_H
