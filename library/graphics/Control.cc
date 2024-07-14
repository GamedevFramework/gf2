// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/Control.h>

#include <gf2/core/ControlSettings.h>
#include <gf2/core/GamepadTypes.h>
#include <gf2/core/MouseTypes.h>

namespace gf {

  namespace {

    using namespace gf::operators;
    constexpr Flags<Modifier> SpecialModifiers = Modifier::Num | Modifier::Caps | Modifier::Mode;

  }

  Control::Control(const ControlSettings& settings)
  : m_settings(settings)
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
    switch (m_settings.type) {
      case gf::ControlType::Keycode:
        process_keycode_control(event, m_settings.keycode);
        break;
      case gf::ControlType::Scancode:
        process_scancode_control(event, m_settings.scancode);
        break;
      case gf::ControlType::MouseButton:
        process_mouse_button_control(event, m_settings.mouse_button);
        break;
      case gf::ControlType::GamepadButton:
        process_gamepad_button_control(event, m_settings.gamepad_button);
        break;
      case gf::ControlType::GamepadAxis:
        process_gamepad_axis_control(event, m_settings.gamepad_axis);
        break;
    }
  }

  void Control::process_keycode_control(const Event& event, const KeycodeControlSettings& settings)
  {
    auto event_modifiers = event.key.modifiers & ~SpecialModifiers;

    if (event.type == EventType::KeyPressed) {
      if (event.key.keycode == settings.keycode && event_modifiers == settings.modifiers) {
        trigger();
      }
    }

    if (event.type == EventType::KeyReleased) {
      if (event.key.keycode == settings.keycode && event_modifiers == settings.modifiers) {
        reset();
      }
    }
  }

  void Control::process_scancode_control(const Event& event, const ScancodeControlSettings& settings)
  {
    auto event_modifiers = event.key.modifiers & ~SpecialModifiers;

    if (event.type == EventType::KeyPressed) {
      if (event.key.scancode == settings.scancode && event_modifiers == settings.modifiers) {
        trigger();
      }
    }

    if (event.type == EventType::KeyReleased) {
      if (event.key.scancode == settings.scancode && event_modifiers == settings.modifiers) {
        reset();
      }
    }
  }

  void Control::process_mouse_button_control(const Event& event, const MouseButtonControlSettings& settings)
  {
    if (event.type == EventType::MouseButtonPressed) {
      if (settings.button == AnyMouseButton || event.mouse_button.button == settings.button) {
        trigger();
      }
    }

    if (event.type == EventType::MouseButtonReleased) {
      if (settings.button == AnyMouseButton || event.mouse_button.button == settings.button) {
        reset();
      }
    }
  }

  void Control::process_gamepad_button_control(const Event& event, const GamepadButtonControlSettings& settings)
  {
    if (event.type == EventType::GamepadButtonPressed) {
      if ((settings.gamepad_id == AnyGamepad || event.gamepad_button.gamepad_id == settings.gamepad_id) && event.gamepad_button.button == settings.button) {
        trigger();
      }
    }

    if (event.type == EventType::GamepadButtonReleased) {
      if ((settings.gamepad_id == AnyGamepad || event.gamepad_button.gamepad_id == settings.gamepad_id) && event.gamepad_button.button == settings.button) {
        reset();
      }
    }
  }

  static constexpr int16_t GamepadAxisThreshold = 8000;

  void Control::process_gamepad_axis_control(const Event& event, const GamepadAxisControlSettings& settings)
  {
    if (event.type != EventType::GamepadAxisMoved) {
      return;
    }

    if ((settings.gamepad_id == AnyGamepad || event.gamepad_axis.gamepad_id == settings.gamepad_id) && event.gamepad_axis.axis == settings.axis) {
      switch (settings.direction) {
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
