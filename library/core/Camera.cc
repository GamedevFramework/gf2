// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Camera.h>
// clang-format on

#include <cmath>

namespace gf {

  void Camera::update(Vec2I framebuffer_size)
  {
    computed_size = expected_size;
    computed_viewport = expected_viewport;

    switch (type) {
      case CameraType::Extend:
        {
          const float world_ratio = expected_size.w / expected_size.h;

          const Vec2F viewport_size = framebuffer_size * expected_viewport.extent;
          const float framebuffer_ratio = viewport_size.w / viewport_size.h;

          Vec2F size = expected_size;

          if (framebuffer_ratio < world_ratio) {
            const float ratio = framebuffer_ratio / world_ratio;
            size.y /= ratio;
          } else {
            const float ratio = world_ratio / framebuffer_ratio;
            size.x /= ratio;
          }

          computed_size = size;
          break;
        }
      case CameraType::Fill:
        {
          const float world_ratio = expected_size.w / expected_size.h;

          const Vec2F viewport_size = framebuffer_size * expected_viewport.extent;
          const float framebuffer_ratio = viewport_size.w / viewport_size.h;

          Vec2F size = expected_size;

          if (framebuffer_ratio < world_ratio) {
            const float ratio = framebuffer_ratio / world_ratio;
            size.x *= ratio;
          } else {
            const float ratio = world_ratio / framebuffer_ratio;
            size.y *= ratio;
          }

          computed_size = size;
          break;
        }
      case CameraType::Fit:
        {
          const float world_ratio = expected_size.w / expected_size.h;

          const Vec2F viewport_size = framebuffer_size * expected_viewport.extent;
          const float framebuffer_ratio = viewport_size.w / viewport_size.h;

          RectF viewport;

          if (framebuffer_ratio < world_ratio) {
            const float ratio = framebuffer_ratio / world_ratio;

            viewport.offset.x = 0.0f;
            viewport.extent.w = 1.0f;

            viewport.offset.y = (1.0f - ratio) / 2.0f;
            viewport.extent.h = ratio;
          } else {
            const float ratio = world_ratio / framebuffer_ratio;

            viewport.offset.y = 0.0f;
            viewport.extent.h = 1.0f;

            viewport.offset.x = (1.0f - ratio) / 2.0f;
            viewport.extent.w = ratio;
          }

          computed_viewport.offset = viewport.offset * expected_viewport.extent + expected_viewport.offset;
          computed_viewport.extent = viewport.extent * expected_viewport.extent;
          break;
        }
      case CameraType::Locked:
        {
          Vec2F size = expected_size;
          const Vec2F viewport_size = framebuffer_size * expected_viewport.extent;

          RectF viewport;

          if (size.w > viewport_size.w) {
            viewport.offset.x = 0.0f;
            viewport.extent.w = 1.0f;
            size.w = viewport_size.w;
          } else {
            viewport.extent.w = size.w / viewport_size.w;
            viewport.offset.x = (1.0f - size.w) / 2.0f;
          }

          if (size.h > viewport_size.h) {
            viewport.offset.y = 0.0f;
            viewport.extent.h = 1.0f;
            size.h = viewport_size.h;
          } else {
            viewport.extent.h = size.h / viewport_size.h;
            viewport.offset.y = (1.0f - size.h) / 2.0f;
          }

          computed_size = size;
          computed_viewport = viewport;
          break;
        }
      case CameraType::Screen:
        {
          const Vec2F viewport_size = framebuffer_size * expected_viewport.extent;

          computed_size = viewport_size;
          center = viewport_size / 2.0f;
          break;
        }
      case CameraType::Stretch:
        /* nothing to do */
        break;
    }
  }

  RectI Camera::compute_viewport(Vec2I framebuffer_size) const
  {
    RectI viewport;
    viewport.offset.x = static_cast<int>(std::lround(static_cast<float>(framebuffer_size.w) * computed_viewport.offset.x));
    viewport.offset.y = static_cast<int>(std::lround(static_cast<float>(framebuffer_size.h) * computed_viewport.offset.y));
    viewport.extent.w = static_cast<int>(std::lround(static_cast<float>(framebuffer_size.w) * computed_viewport.extent.w));
    viewport.extent.h = static_cast<int>(std::lround(static_cast<float>(framebuffer_size.h) * computed_viewport.extent.h));
    return viewport;
  }

  Mat3F Camera::compute_view_matrix() const
  {
    const float sx = 2.0f / computed_size.w;
    const float sy = 2.0f / computed_size.h;
    const float cos_v = std::cos(rotation);
    const float sin_v = std::sin(rotation);
    const float tx = center.x;
    const float ty = center.y;

    Mat3F mat;
    // clang-format off
    mat(0, 0) = sx * cos_v;  mat(0, 1) = sx * sin_v; mat(0, 2) = sx * (-tx * cos_v - ty * sin_v);
    mat(1, 0) = -sy * sin_v; mat(1, 1) = sy * cos_v; mat(1, 2) = sy * (tx * sin_v - ty * cos_v);
    mat(2, 0) = 0.0f;        mat(2, 1) = 0.0f;       mat(2, 2) = 1.0f;
    // clang-format on
    return mat;
  }

}
