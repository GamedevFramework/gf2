// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SHAPE_ENTITY_H
#define GF_SHAPE_ENTITY_H

#include <gf2/core/Transform.h>

#include "Entity.h"
#include "GraphicsApi.h"
#include "Shape.h"
#include "Texture.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API ShapeEntity : public Entity {
  public:
    ShapeEntity(const Texture* texture, const ShapeData& data, RenderManager* render_manager);

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
    Shape m_shape;
    Transform m_transform;
  };

}

#endif // GF_SHAPE_ENTITY_H
