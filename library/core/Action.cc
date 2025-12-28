// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Action.h>

#include <algorithm>

namespace gf {

  Action::Action(ActionType type)
  : m_type(type)
  {
  }

  Action::Action(const ActionSettings& settings)
  : m_type(settings.type)
  {
    for (const ControlSettings& control_settings : settings.controls) {
      add_control_settings(control_settings);
    }
  }

  void Action::set_continuous()
  {
    m_type = ActionType::Continuous;
  }

  bool Action::continuous() const
  {
    return m_type == ActionType::Continuous;
  }

  void Action::set_instantaneous()
  {
    m_type = ActionType::Instantaneous;
  }

  bool Action::instantaneous() const
  {
    return m_type == ActionType::Instantaneous;
  }

  void Action::add_keycode_control(Keycode keycode, Flags<Modifier> modifiers)
  {
    add_control_settings({ keycode, modifiers });
  }

  void Action::add_scancode_control(Scancode scancode, Flags<Modifier> modifiers)
  {
    add_control_settings({ scancode, modifiers });
  }

  void Action::add_mouse_button_control(MouseButton button)
  {
    add_control_settings({ button });
  }

  void Action::add_gamepad_button_control(GamepadId gamepad_id, GamepadButton button)
  {
    add_control_settings({ gamepad_id, button });
  }

  void Action::add_gamepad_axis_control(GamepadId gamepad_id, GamepadAxis axis, GamepadAxisDirection direction)
  {
    add_control_settings({ gamepad_id, axis, direction });
  }

  void Action::add_control_settings(const ControlSettings& settings)
  {
    add_control(Control(settings));
  }

  void Action::add_control(const Control& control)
  {
    m_controls.push_back(control);
  }

  bool Action::active() const
  {
    return std::ranges::any_of(m_controls, &Control::active);
  }

  void Action::reset()
  {
    if (continuous()) {
      return;
    }

    for (Control& control : m_controls) {
      control.reset();
    }
  }

  void Action::hard_reset()
  {
    for (Control& control : m_controls) {
      control.reset();
    }
  }

  void Action::process_event(const Event& event)
  {
    for (Control& control : m_controls) {
      control.process_event(event);
    }
  }

}
