// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/core/Camera.h>
// clang-format on

#include <cmath>

namespace gf {

  void Camera::update(Vec2I target_size)
  {
    m_computed_size = size;
    m_computed_viewport = viewport;

    switch (type) {
      case CameraType::Extend:
        {
          const float world_ratio = size.w / size.h;

          const Vec2F viewport_size = target_size * viewport.extent;
          const float framebuffer_ratio = viewport_size.w / viewport_size.h;

          if (framebuffer_ratio < world_ratio) {
            const float ratio = framebuffer_ratio / world_ratio;
            m_computed_size.y /= ratio;
          } else {
            const float ratio = world_ratio / framebuffer_ratio;
            m_computed_size.x /= ratio;
          }

          break;
        }
      case CameraType::Fill:
        {
          const float world_ratio = size.w / size.h;

          const Vec2F viewport_size = target_size * viewport.extent;
          const float framebuffer_ratio = viewport_size.w / viewport_size.h;

          if (framebuffer_ratio < world_ratio) {
            const float ratio = framebuffer_ratio / world_ratio;
            m_computed_size.x *= ratio;
          } else {
            const float ratio = world_ratio / framebuffer_ratio;
            m_computed_size.y *= ratio;
          }

          break;
        }
      case CameraType::Fit:
        {
          const float world_ratio = size.w / size.h;

          const Vec2F viewport_size = target_size * viewport.extent;
          const float framebuffer_ratio = viewport_size.w / viewport_size.h;

          RectF basic_viewport;

          if (framebuffer_ratio < world_ratio) {
            const float ratio = framebuffer_ratio / world_ratio;

            basic_viewport.offset.x = 0.0f;
            basic_viewport.extent.w = 1.0f;

            basic_viewport.offset.y = (1.0f - ratio) / 2.0f;
            basic_viewport.extent.h = ratio;
          } else {
            const float ratio = world_ratio / framebuffer_ratio;

            basic_viewport.offset.y = 0.0f;
            basic_viewport.extent.h = 1.0f;

            basic_viewport.offset.x = (1.0f - ratio) / 2.0f;
            basic_viewport.extent.w = ratio;
          }

          m_computed_viewport.offset = basic_viewport.offset * viewport.extent + viewport.offset;
          m_computed_viewport.extent = basic_viewport.extent * viewport.extent;
          break;
        }
      case CameraType::Locked:
        {
          const Vec2F viewport_size = target_size * viewport.extent;

          RectF basic_viewport;

          if (m_computed_size.w > viewport_size.w) {
            basic_viewport.offset.x = 0.0f;
            basic_viewport.extent.w = 1.0f;
            m_computed_size.w = viewport_size.w;
          } else {
            basic_viewport.extent.w = m_computed_size.w / viewport_size.w;
            basic_viewport.offset.x = (1.0f - m_computed_size.w) / 2.0f;
          }

          if (m_computed_size.h > viewport_size.h) {
            basic_viewport.offset.y = 0.0f;
            basic_viewport.extent.h = 1.0f;
            m_computed_size.h = viewport_size.h;
          } else {
            basic_viewport.extent.h = m_computed_size.h / viewport_size.h;
            basic_viewport.offset.y = (1.0f - m_computed_size.h) / 2.0f;
          }

          m_computed_viewport = basic_viewport;
          break;
        }
      case CameraType::Window:
        {
          const Vec2F viewport_size = target_size * viewport.extent;

          m_computed_size = viewport_size;
          center = viewport_size / 2.0f;
          break;
        }
      case CameraType::Stretch:
        /* nothing to do */
        break;
    }
  }

  RectI Camera::compute_viewport(Vec2I target_size) const
  {
    RectI actual_viewport;
    actual_viewport.offset.x = static_cast<int>(std::lround(static_cast<float>(target_size.w) * m_computed_viewport.offset.x));
    actual_viewport.offset.y = static_cast<int>(std::lround(static_cast<float>(target_size.h) * m_computed_viewport.offset.y));
    actual_viewport.extent.w = static_cast<int>(std::lround(static_cast<float>(target_size.w) * m_computed_viewport.extent.w));
    actual_viewport.extent.h = static_cast<int>(std::lround(static_cast<float>(target_size.h) * m_computed_viewport.extent.h));
    return actual_viewport;
  }

  Mat3F Camera::compute_view_matrix() const
  {
    const float sx = 2.0f / m_computed_size.w;
    const float sy = 2.0f / m_computed_size.h;
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
