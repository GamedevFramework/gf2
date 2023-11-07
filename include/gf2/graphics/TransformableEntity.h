// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TRANSFORMABLE_ENTITY_H
#define GF_TRANSFORMABLE_ENTITY_H

#include <gf2/core/Transform.h>

#include "GraphicsApi.h"
#include "Entity.h"

namespace gf {

  class GF_GRAPHICS_API TransformableEntity : public Entity {
  public:
    TransformableEntity(int32_t priority = 0);

    void set_location(Vec2F location);
    Vec2F location() const;

    void set_origin(Vec2F origin);
    Vec2F origin() const;

    void set_rotation(float rotation);
    float rotation() const;

    void set_scale(float scale);
    void set_scale(Vec2F scale);
    Vec2F scale() const;

    Transform transform() const;

  protected:
    Mat3F compute_matrix(RectF bounds) const;

  private:
    Transform m_transform;
  };

}

#endif // GF_TRANSFORMABLE_ENTITY_H
