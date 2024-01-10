// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Control.h>
// clang-format on

#include <gf2/core/ControlData.h>
#include <gf2/core/GamepadTypes.h>
#include <gf2/core/MouseTypes.h>

namespace gf {

  namespace {

    using namespace gf::operators;
    constexpr Flags<Modifier> SpecialModifiers = Modifier::Num | Modifier::Caps | Modifier::Mode;

  }

  Control::Control(const ControlData& data)
  : m_data(data)
  {
  }

  bool Control::active() const
  {
    return m_active;
  }

  void Control::trigger()
  {
    m_active = true;
  }

  void Control::reset()
  {
    m_active = false;
  }

  void Control::process_event(const Event& event)
  {
    switch (m_data.type) {
      case gf::ControlType::Keycode:
        process_keycode_control(event, m_data.keycode);
        break;
      case gf::ControlType::Scancode:
        process_scancode_control(event, m_data.scancode);
        break;
      case gf::ControlType::MouseButton:
        process_mouse_button_control(event, m_data.mouse_button);
        break;
      case gf::ControlType::GamepadButton:
        process_gamepad_button_control(event, m_data.gamepad_button);
        break;
      case gf::ControlType::GamepadAxis:
        process_gamepad_axis_control(event, m_data.gamepad_axis);
        break;
    }
  }

  void Control::process_keycode_control(const Event& event, const KeycodeControlData& data)
  {
    auto event_modifiers = event.key.modifiers & ~SpecialModifiers;

    if (event.type == EventType::KeyPressed) {
      if (event.key.keycode == data.keycode && event_modifiers == data.modifiers) {
        trigger();
      }
    }

    if (event.type == EventType::KeyReleased) {
      if (event.key.keycode == data.keycode && event_modifiers == data.modifiers) {
        reset();
      }
    }
  }

  void Control::process_scancode_control(const Event& event, const ScancodeControlData& data)
  {
    auto event_modifiers = event.key.modifiers & ~SpecialModifiers;

    if (event.type == EventType::KeyPressed) {
      if (event.key.scancode == data.scancode && event_modifiers == data.modifiers) {
        trigger();
      }
    }

    if (event.type == EventType::KeyReleased) {
      if (event.key.scancode == data.scancode && event_modifiers == data.modifiers) {
        reset();
      }
    }
  }

  void Control::process_mouse_button_control(const Event& event, const MouseButtonControlData& data)
  {
    if (event.type == EventType::MouseButtonPressed) {
      if (data.button == AnyMouseButton || event.mouse_button.button == data.button) {
        trigger();
      }
    }

    if (event.type == EventType::MouseButtonReleased) {
      if (data.button == AnyMouseButton || event.mouse_button.button == data.button) {
        reset();
      }
    }
  }

  void Control::process_gamepad_button_control(const Event& event, const GamepadButtonControlData& data)
  {
    if (event.type == EventType::GamepadButtonPressed) {
      if ((data.gamepad_id == AnyGamepad || event.gamepad_button.gamepad_id == data.gamepad_id) && event.gamepad_button.button == data.button) {
        trigger();
      }
    }

    if (event.type == EventType::GamepadButtonReleased) {
      if ((data.gamepad_id == AnyGamepad || event.gamepad_button.gamepad_id == data.gamepad_id) && event.gamepad_button.button == data.button) {
        reset();
      }
    }
  }

  static constexpr int16_t GamepadAxisThreshold = 8000;

  void Control::process_gamepad_axis_control(const Event& event, const GamepadAxisControlData& data)
  {
    if (event.type != EventType::GamepadAxisMoved) {
      return;
    }

    if ((data.gamepad_id == AnyGamepad || event.gamepad_axis.gamepad_id == data.gamepad_id) && event.gamepad_axis.axis == data.axis) {
      switch (data.direction) {
        case gf::GamepadAxisDirection::Positive:
          if (event.gamepad_axis.value > GamepadAxisThreshold) {
            if (!m_repeated) {
              trigger();
            }

            m_repeated = true;
          } else {
            reset();
            m_repeated = false;
          }

          break;
        case gf::GamepadAxisDirection::Negative:
          if (event.gamepad_axis.value < -GamepadAxisThreshold) {
            if (!m_repeated) {
              trigger();
            }

            m_repeated = true;
          } else {
            reset();
            m_repeated = false;
          }

          break;
      }
    }
  }

}
