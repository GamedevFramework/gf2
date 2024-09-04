// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/MotionGroup.h>

#include <cassert>

#include <algorithm>

#include <gf2/graphics/Event.h>
#include "gf2/core/GamepadTypes.h"

namespace gf {

  namespace {

    float normalize_stick_axis_value(int16_t value)
    {
      static constexpr int16_t Max = std::numeric_limits<int16_t>::max();
      static constexpr int16_t Min = std::numeric_limits<int16_t>::min() + 1;
      value = std::clamp(value, Min, Max);
      auto normalized_value = static_cast<float>(value) / Max;
      assert(-1.0f <= normalized_value && normalized_value <= 1.0f);
      return normalized_value;
    }

  }

  Vec2I MotionGroup::last_mouse_position() const
  {
    return m_last_mouse_position;
  }

  Vec2I MotionGroup::last_mouse_motion() const
  {
    return m_last_mouse_motion;
  }

  Vec2I MotionGroup::last_mouse_button_pressed_position(MouseButton button) const
  {
    auto index = static_cast<std::size_t>(button);

    if (index >= m_last_mouse_button_pressed_position.size()) {
      return { 0, 0 };
    }

    return m_last_mouse_button_pressed_position[index]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
  }

  Vec2I MotionGroup::last_mouse_button_released_position(MouseButton button) const
  {
    auto index = static_cast<std::size_t>(button);

    if (index >= m_last_mouse_button_released_position.size()) {
      return { 0, 0 };
    }

    return m_last_mouse_button_released_position[index]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
  }

  Vec2I MotionGroup::last_mouse_wheel_offset() const
  {
    return m_last_mouse_wheel_offset;
  }

  Vec2F MotionGroup::last_gamepad_stick_location(GamepadStick stick) const
  {
    auto index = static_cast<std::size_t>(stick);

    if (index >= m_last_gamepad_stick_location.size()) {
      return { 0.0f, 0.0f };
    }

    return m_last_gamepad_stick_location[index]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
  }

  Vec2F MotionGroup::last_touch_location() const
  {
    return m_last_touch_location;
  }

  Vec2F MotionGroup::last_touch_motion() const
  {
    return m_last_touch_motion;
  }

  void MotionGroup::process_event(const Event& event)
  {
    switch (event.type) {
      case EventType::MouseMoved:
        m_last_mouse_position = event.mouse_motion.position;
        m_last_mouse_motion = event.mouse_motion.motion;
        break;
      case EventType::MouseButtonPressed:
        {
          auto index = static_cast<std::size_t>(event.mouse_button.button);
          assert(index < m_last_mouse_button_pressed_position.size());
          m_last_mouse_button_pressed_position[index] = event.mouse_button.position; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        }
        break;
      case EventType::MouseButtonReleased:
        {
          auto index = static_cast<std::size_t>(event.mouse_button.button);
          assert(index < m_last_mouse_button_released_position.size());
          m_last_mouse_button_released_position[index] = event.mouse_button.position; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        }
        break;
      case EventType::MouseWheelScrolled:
        m_last_mouse_wheel_offset = event.mouse_wheel.offset;
        break;
      case EventType::GamepadAxisMoved:
        switch (event.gamepad_axis.axis) {
          case gf::GamepadAxis::LeftX:
            m_last_gamepad_stick_location[0].x = normalize_stick_axis_value(event.gamepad_axis.value);
            break;
          case gf::GamepadAxis::LeftY:
            m_last_gamepad_stick_location[0].y = normalize_stick_axis_value(event.gamepad_axis.value);
            break;
          case gf::GamepadAxis::RightX:
            m_last_gamepad_stick_location[1].x = normalize_stick_axis_value(event.gamepad_axis.value);
            break;
          case gf::GamepadAxis::RightY:
            m_last_gamepad_stick_location[1].y = normalize_stick_axis_value(event.gamepad_axis.value);
            break;
          default:
            break;
        }
        break;
      case EventType::TouchMoved:
        m_last_touch_location = event.touch.location;
        m_last_touch_motion = event.touch.motion;
        break;
      default:
        break;
    }
  }

}
