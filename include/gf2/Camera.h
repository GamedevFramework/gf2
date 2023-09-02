// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CAMERA_H
#define GF_CAMERA_H

#include <cstdint>

#include "CoreApi.h"
#include "Mat3.h"
#include "Rect.h"
#include "Vec2.h"

namespace gf {

  enum class CameraType : uint8_t {
    Extend,
    Fill,
    Fit,
    Locked,
    Screen,
    Stretch,
  };

  class GF_CORE_API Camera {
  public:
    CameraType type = CameraType::Screen;
    Vec2F center = { 0.0f, 0.0f };
    Vec2F size = { 2.0f, 2.0f };
    float rotation = 0.0f;
    // clang-format off
    RectF viewport = {{ 0.0f, 0.0f }, { 1.0f, 1.0f }};
    // clang-format on

    Camera() = default;

    constexpr Camera(CameraType type, Vec2F center, Vec2F size)
    : type(type)
    , center(center)
    , size(size)
    , m_computed_size(size)
    {
    }

    void update(Vec2I framebuffer_size);
    RectI compute_viewport(Vec2I framebuffer_size) const;
    Mat3F compute_view_matrix() const;

  private:
    Vec2F m_computed_size = { 2.0f, 2.0f };
    // clang-format off
    RectF m_computed_viewport = {{ 0.0f, 0.0f }, { 1.0f, 1.0f }};
    // clang-format on
  };

}

#endif // GF_CAMERA_H
