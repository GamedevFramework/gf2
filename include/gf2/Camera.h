// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CAMERA_H
#define GF_CAMERA_H

#include "CoreApi.h"
#include "Mat3.h"
#include "Rect.h"
#include "Vec2.h"

namespace gf {

  enum class CameraType {
    Extend,
    Fill,
    Fit,
    Locked,
    Screen,
    Stretch,
  };

  struct GF_CORE_API Camera {
    CameraType type = CameraType::Screen;
    Vec2F center = { 0.0f, 0.0f };
    Vec2F expected_size = { 2.0f, 2.0f };
    Vec2F computed_size = { 2.0f, 2.0f };
    float rotation = 0.0f;
    RectF expected_viewport = {{ 0.0f, 0.0f }, { 1.0f, 1.0f }};
    RectF computed_viewport = {{ 0.0f, 0.0f }, { 1.0f, 1.0f }};

    Camera() = default;

    constexpr Camera(CameraType type, Vec2F center, Vec2F size)
    : type(type)
    , center(center)
    , expected_size(size)
    , computed_size(size)
    , rotation(0.0f)
    , expected_viewport({{ 0.0f, 0.0f }, { 1.0f, 1.0f }})
    , computed_viewport({{ 0.0f, 0.0f }, { 1.0f, 1.0f }})
    {
    }

    void update(Vec2I framebuffer_size);
    RectI compute_viewport(Vec2I framebuffer_size) const;
    Mat3F compute_view_matrix() const;
  };

}

#endif // GF_CAMERA_H

