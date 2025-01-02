// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_TRANSFORM_H
#define GF_TRANSFORM_H

#include "CoreApi.h"
#include "Mat3.h"
#include "Rect.h"
#include "Vec2.h"

namespace gf {

  struct GF_CORE_API Transform {
    constexpr Transform(Vec2F location = { 0.0f, 0.0f }, Vec2F origin = { 0.0f, 0.0f }, float rotation = 0.0f)
    : location(location)
    , origin(origin)
    , rotation(rotation)
    {
    }

    Vec2F location = { 0.0f, 0.0f };
    Vec2F origin = { 0.0f, 0.0f };
    float rotation = 0.0f;
    Vec2F scale = { 1.0f, 1.0f };

    Mat3F compute_matrix(RectF bounds) const;
    Mat3F compute_inverse_matrix(RectF bounds) const;
  };

  GF_CORE_API bool transformed_contains(RectF object_bounds, const Transform& object_transform, Vec2F location);

} // namespace gf

#endif // GF_TRANSFORM_H
