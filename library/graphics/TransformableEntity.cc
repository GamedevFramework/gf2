// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/TransformableEntity.h>
// clang-format on

namespace gf {
  TransformableEntity::TransformableEntity(int32_t priority)
  : Entity(priority)
  {
  }

  void TransformableEntity::set_location(Vec2F location)
  {
    m_transform.location = location;
  }

  Vec2F TransformableEntity::location() const
  {
    return m_transform.location;
  }

  void TransformableEntity::set_origin(Vec2F origin)
  {
    m_transform.origin = origin;
  }

  Vec2F TransformableEntity::origin() const
  {
    return m_transform.origin;
  }

  void TransformableEntity::set_rotation(float rotation)
  {
    m_transform.rotation = rotation;
  }

  float TransformableEntity::rotation() const
  {
    return m_transform.rotation;
  }

  void TransformableEntity::set_scale(float scale)
  {
    set_scale({ scale, scale });
  }

  void TransformableEntity::set_scale(Vec2F scale)
  {
    m_transform.scale = scale;
  }

  Vec2F TransformableEntity::scale() const
  {
    return m_transform.scale;
  }

  void TransformableEntity::set_transform(const Transform& transform)
  {
    m_transform = transform;
  }

  const Transform& TransformableEntity::transform() const
  {
    return m_transform;
  }

  Mat3F TransformableEntity::compute_matrix(RectF bounds) const
  {
    return m_transform.compute_matrix(bounds);
  }

}
