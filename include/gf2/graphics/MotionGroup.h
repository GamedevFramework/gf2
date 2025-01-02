// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_MOTION_GROUP_H
#define GF_MOTION_GROUP_H

#include <array>

#include <gf2/core/GamepadTypes.h>
#include <gf2/core/MouseTypes.h>
#include <gf2/core/Vec2.h>

#include "GraphicsApi.h"

namespace gf {
  struct Event;

  class GF_GRAPHICS_API MotionGroup {
  public:

    Vec2I last_mouse_position() const;
    Vec2I last_mouse_motion() const;
    Vec2I last_mouse_button_pressed_position(MouseButton button) const;
    Vec2I last_mouse_button_released_position(MouseButton button) const;
    Vec2I last_mouse_wheel_offset() const;
    Vec2F last_gamepad_stick_location(GamepadStick stick) const;
    Vec2F last_touch_location() const;
    Vec2F last_touch_motion() const;

    void process_event(const Event& event);

  private:
    Vec2I m_last_mouse_position = {};
    Vec2I m_last_mouse_motion = {};
    std::array<Vec2I, 8> m_last_mouse_button_pressed_position = {};
    std::array<Vec2I, 8> m_last_mouse_button_released_position = {};
    Vec2I m_last_mouse_wheel_offset = {};
    std::array<Vec2F, 2> m_last_gamepad_stick_location = {};
    Vec2F m_last_touch_location = {};
    Vec2F m_last_touch_motion = {};
  };

}

#endif // GF_MOTION_GROUP_H
