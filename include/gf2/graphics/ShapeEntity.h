// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SHAPE_ENTITY_H
#define GF_SHAPE_ENTITY_H

#include "GraphicsApi.h"
#include "Shape.h"
#include "Texture.h"
#include "TransformableEntity.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API ShapeEntity : public TransformableEntity {
  public:
    ShapeEntity(const Texture* texture, const ShapeBuffer& buffer, RenderManager* render_manager);
    ShapeEntity(const ShapeResource& resource, RenderManager* render_manager, ResourceManager* resource_manager);

    void render(RenderRecorder& recorder) override;

    const Shape& shape() const
    {
      return m_shape;
    }

  private:
    Shape m_shape;
  };

}

#endif // GF_SHAPE_ENTITY_H
