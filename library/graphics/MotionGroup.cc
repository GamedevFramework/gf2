// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

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
    switch (event.type()) {
      case EventType::MouseMoved:
        {
          const auto& mouse_moved_event = event.from<EventType::MouseMoved>();
          m_last_mouse_position = mouse_moved_event.position;
          m_last_mouse_motion = mouse_moved_event.motion;
        }
        break;
      case EventType::MouseButtonPressed:
        {
          const auto& mouse_button_pressed = event.from<EventType::MouseButtonPressed>();
          auto index = static_cast<std::size_t>(mouse_button_pressed.button);
          assert(index < m_last_mouse_button_pressed_position.size());
          m_last_mouse_button_pressed_position[index] = mouse_button_pressed.position; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        }
        break;
      case EventType::MouseButtonReleased:
        {
          const auto& mouse_button_released = event.from<EventType::MouseButtonReleased>();
          auto index = static_cast<std::size_t>(mouse_button_released.button);
          assert(index < m_last_mouse_button_released_position.size());
          m_last_mouse_button_released_position[index] = mouse_button_released.position; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        }
        break;
      case EventType::MouseWheelScrolled:
        m_last_mouse_wheel_offset = event.from<EventType::MouseWheelScrolled>().offset;
        break;
      case EventType::GamepadAxisMoved:
        {
          const auto& gamepad_axis_moved_event = event.from<EventType::GamepadAxisMoved>();

          switch (gamepad_axis_moved_event.axis) {
            case gf::GamepadAxis::LeftX:
              m_last_gamepad_stick_location[0].x = normalize_stick_axis_value(gamepad_axis_moved_event.value);
              break;
            case gf::GamepadAxis::LeftY:
              m_last_gamepad_stick_location[0].y = normalize_stick_axis_value(gamepad_axis_moved_event.value);
              break;
            case gf::GamepadAxis::RightX:
              m_last_gamepad_stick_location[1].x = normalize_stick_axis_value(gamepad_axis_moved_event.value);
              break;
            case gf::GamepadAxis::RightY:
              m_last_gamepad_stick_location[1].y = normalize_stick_axis_value(gamepad_axis_moved_event.value);
              break;
            default:
              break;
          }
        }
        break;
      case EventType::TouchMoved:
        {
          const auto& touch_moved_event = event.from<EventType::TouchMoved>();
          m_last_touch_location = touch_moved_event.location;
          m_last_touch_motion = touch_moved_event.motion;
        }
        break;
      default:
        break;
    }
  }

}
