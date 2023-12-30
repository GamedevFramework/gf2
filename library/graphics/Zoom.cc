// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Zoom.h>
// clang-format on

namespace gf {

  Zoom::Zoom(Camera* camera)
  : m_camera(camera)
  , m_mouse_position(0, 0)
  , m_surface_size(0, 0)
  {
  }

  void Zoom::set_surface_size(Vec2I surface_size)
  {
    m_surface_size = surface_size;
  }

  void Zoom::process_event(const Event& event)
  {
    static constexpr float ZoomInFactor = 0.8f;
    static constexpr float ZoomOutFactor = 1.25f;

    switch (event.type) {
      case EventType::MouseMoved:
        if (m_state == State::Moving) {
          auto old_position = m_camera->position_to_location(m_mouse_position, m_surface_size);
          auto new_position = m_camera->position_to_location(event.mouse_motion.position, m_surface_size);
          m_camera->move(old_position - new_position);
        }

        m_mouse_position = event.mouse_motion.position;
        break;

      case EventType::MouseButtonPressed:
        m_state = State::Moving;
        break;

      case EventType::MouseButtonReleased:
        m_state = State::Stationary;
        break;

      case EventType::MouseWheelScrolled:
        if (event.mouse_wheel.offset.y > 0) {
          m_camera->zoom(ZoomInFactor, m_camera->position_to_location(m_mouse_position, m_surface_size));
        } else {
          m_camera->zoom(ZoomOutFactor, m_camera->position_to_location(m_mouse_position, m_surface_size));
        }
        break;

      default:
        break;
    }
  }

}
