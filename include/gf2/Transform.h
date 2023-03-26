// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TRANSFORM_H
#define GF_TRANSFORM_H

#include "Mat3.h"
#include "Rect.h"
#include "Vec2.h"

namespace gf {

  struct Transform {
    constexpr Transform(Vec2F position = gf::vec(0.0f, 0.0f), Vec2F origin = gf::vec(0.0f, 0.0f), float rotation = 0.0f)
    : position(position)
    , origin(origin)
    , rotation(rotation)
    {
    }

    Vec2F position = gf::vec(0.0f, 0.0f);
    Vec2F origin = gf::vec(0.0f, 0.0f);
    float rotation = 0.0f;
    Vec2F scale = gf::vec(1.0f, 1.0f);

    Mat3F compute_matrix(RectF bounds) const;
  };

} // namespace gf

#endif // GF_TRANSFORM_H
