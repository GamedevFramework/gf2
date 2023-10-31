// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TEXT_ENTITY_H
#define GF_TEXT_ENTITY_H

#include <gf2/core/Transform.h>

#include "Entity.h"
#include "GraphicsApi.h"
#include "Text.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API TextEntity : public Entity {
  public:
    TextEntity(FontAtlas* atlas, const TextData& data, RenderManager* render_manager);

    Transform& transform()
    {
      return m_transform;
    }

    const Transform& transform() const
    {
      return m_transform;
    }

    void render(RenderRecorder& recorder) override;

  private:
    Text m_text;
    Transform m_transform;
  };

}

#endif // GF_TEXT_ENTITY_H
