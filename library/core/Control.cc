// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Control.h>

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
    switch (m_settings.type()) {
      case gf::ControlType::None:
        break;
      case gf::ControlType::Keycode:
        process_keycode_control(event, m_settings.as<KeycodeControlSettings>());
        break;
      case gf::ControlType::Scancode:
        process_scancode_control(event, m_settings.as<ScancodeControlSettings>());
        break;
      case gf::ControlType::MouseButton:
        process_mouse_button_control(event, m_settings.as<MouseButtonControlSettings>());
        break;
      case gf::ControlType::GamepadButton:
        process_gamepad_button_control(event, m_settings.as<GamepadButtonControlSettings>());
        break;
      case gf::ControlType::GamepadAxis:
        process_gamepad_axis_control(event, m_settings.as<GamepadAxisControlSettings>());
        break;
    }
  }

  void Control::process_keycode_control(const Event& event, const KeycodeControlSettings& settings)
  {
    if (event.type() == EventType::KeyPressed) {
      const KeyPressedEvent& key_pressed_event = event.from<EventType::KeyPressed>();
      const Flags<Modifier> event_modifiers = key_pressed_event.modifiers & ~SpecialModifiers;

      if (key_pressed_event.keycode == settings.keycode && event_modifiers == settings.modifiers) {
        trigger();
      }
    }

    if (event.type() == EventType::KeyReleased) {
      const KeyReleasedEvent& key_released_event = event.from<EventType::KeyReleased>();
      const Flags<Modifier> event_modifiers = key_released_event.modifiers & ~SpecialModifiers;

      if (key_released_event.keycode == settings.keycode && event_modifiers == settings.modifiers) {
        reset();
      }
    }
  }

  void Control::process_scancode_control(const Event& event, const ScancodeControlSettings& settings)
  {
    if (event.type() == EventType::KeyPressed) {
      const KeyPressedEvent& key_pressed_event = event.from<EventType::KeyPressed>();
      const Flags<Modifier> event_modifiers = key_pressed_event.modifiers & ~SpecialModifiers;

      if (key_pressed_event.scancode == settings.scancode && event_modifiers == settings.modifiers) {
        trigger();
      }
    }

    if (event.type() == EventType::KeyReleased) {
      const KeyReleasedEvent& key_released_event = event.from<EventType::KeyReleased>();
      const Flags<Modifier> event_modifiers = key_released_event.modifiers & ~SpecialModifiers;

      if (key_released_event.scancode == settings.scancode && event_modifiers == settings.modifiers) {
        reset();
      }
    }
  }

  void Control::process_mouse_button_control(const Event& event, const MouseButtonControlSettings& settings)
  {
    if (event.type() == EventType::MouseButtonPressed) {
      const MouseButtonPressedEvent& mouse_button_pressed_event = event.from<EventType::MouseButtonPressed>();

      if (settings.button == AnyMouseButton || mouse_button_pressed_event.button == settings.button) {
        trigger();
      }
    }

    if (event.type() == EventType::MouseButtonReleased) {
      const MouseButtonReleasedEvent& mouse_button_released_event = event.from<EventType::MouseButtonReleased>();

      if (settings.button == AnyMouseButton || mouse_button_released_event.button == settings.button) {
        reset();
      }
    }
  }

  void Control::process_gamepad_button_control(const Event& event, const GamepadButtonControlSettings& settings)
  {
    if (event.type() == EventType::GamepadButtonPressed) {
      const GamepadButtonPressedEvent& gamepad_button_pressed_event = event.from<EventType::GamepadButtonPressed>();

      if ((settings.gamepad_id == AnyGamepad || gamepad_button_pressed_event.gamepad_id == settings.gamepad_id) && gamepad_button_pressed_event.button == settings.button) {
        trigger();
      }
    }

    if (event.type() == EventType::GamepadButtonReleased) {
      const GamepadButtonReleasedEvent& gamepad_button_released_event = event.from<EventType::GamepadButtonReleased>();

      if ((settings.gamepad_id == AnyGamepad || gamepad_button_released_event.gamepad_id == settings.gamepad_id) && gamepad_button_released_event.button == settings.button) {
        reset();
      }
    }
  }

  static constexpr int16_t GamepadAxisThreshold = 8000;

  void Control::process_gamepad_axis_control(const Event& event, const GamepadAxisControlSettings& settings)
  {
    if (event.type() != EventType::GamepadAxisMoved) {
      return;
    }

    const GamepadAxisMovedEvent& gamepad_axis_moved_event = event.from<EventType::GamepadAxisMoved>();

    if ((settings.gamepad_id == AnyGamepad || gamepad_axis_moved_event.gamepad_id == settings.gamepad_id) && gamepad_axis_moved_event.axis == settings.axis) {
      switch (settings.direction) {
        case gf::GamepadAxisDirection::Positive:
          if (gamepad_axis_moved_event.value > GamepadAxisThreshold) {
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
          if (gamepad_axis_moved_event.value < -GamepadAxisThreshold) {
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
