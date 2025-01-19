// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
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
    Stretch,
    Window,
  };

  class GF_CORE_API Camera {
  public:
    // NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)
    CameraType type = CameraType::Window;
    Vec2F center = { 0.0f, 0.0f };
    Vec2F size = { 2.0f, 2.0f };
    float rotation = 0.0f;
    RectF viewport = RectF::from_size({ 1.0f, 1.0f });
    // NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)

    Camera() = default;

    constexpr Camera(CameraType type, Vec2F center, Vec2F size)
    : type(type)
    , center(center)
    , size(size)
    , m_computed_size(size)
    {
    }

    void move(Vec2F offset);
    void rotate(float angle);
    void zoom(float factor);
    void zoom(float factor, Vec2F fixed);

    void update(Vec2I target_size);
    RectF compute_viewport(Vec2I target_size) const;
    Mat3F compute_view_matrix() const;

    Vec2F position_to_location(Vec2I position, Vec2I target_size) const;
    Vec2I location_to_position(Vec2F location, Vec2I target_size) const;

  private:
    Vec2F m_computed_size = { 2.0f, 2.0f };
    RectF m_computed_viewport = RectF::from_size({ 1.0f, 1.0f });
  };

}

#endif // GF_CAMERA_H
