// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TEXT_ENTITY_H
#define GF_TEXT_ENTITY_H

#include <gf2/core/Transform.h>

#include "GraphicsApi.h"
#include "Text.h"
#include "TransformableEntity.h"

namespace gf {
  class RenderManager;
  class ResourceManager;

  class GF_GRAPHICS_API TextEntity : public TransformableEntity {
  public:
    TextEntity(FontAtlas* atlas, const TextData& data, RenderManager* render_manager);
    TextEntity(const TextResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    Text& text()
    {
      return m_text;
    }

    void render(RenderRecorder& recorder) override;

  private:
    Text m_text;
  };

}

#endif // GF_TEXT_ENTITY_H
