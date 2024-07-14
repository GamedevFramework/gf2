// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/Action.h>

#include <algorithm>
#include <functional>

namespace gf {

  Action::Action(ActionType type)
  : m_type(type)
  {
  }

  Action::Action(const ActionSettings& settings)
  : m_type(settings.type)
  {
    for (const auto& control : settings.controls) {
      add_control(control);
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
    ControlSettings settings = {};
    settings.type = ControlType::Keycode;
    settings.keycode.keycode = keycode;
    settings.keycode.modifiers = modifiers;
    add_control(settings);
  }

  void Action::add_scancode_control(Scancode scancode, Flags<Modifier> modifiers)
  {
    ControlSettings settings = {};
    settings.type = ControlType::Scancode;
    settings.scancode.scancode = scancode;
    settings.scancode.modifiers = modifiers;
    add_control(settings);
  }

  void Action::add_mouse_button_control(MouseButton button)
  {
    ControlSettings settings = {};
    settings.type = ControlType::MouseButton;
    settings.mouse_button.button = button;
    add_control(settings);
  }

  void Action::add_gamepad_button_control(GamepadId gamepad_id, GamepadButton button)
  {
    ControlSettings settings = {};
    settings.type = ControlType::GamepadButton;
    settings.gamepad_button.gamepad_id = gamepad_id;
    settings.gamepad_button.button = button;
    add_control(settings);
  }

  void Action::add_gamepad_axis_control(GamepadId gamepad_id, GamepadAxis axis, GamepadAxisDirection direction)
  {
    ControlSettings settings = {};
    settings.type = ControlType::GamepadAxis;
    settings.gamepad_axis.gamepad_id = gamepad_id;
    settings.gamepad_axis.axis = axis;
    settings.gamepad_axis.direction = direction;
    add_control(settings);
  }

  void Action::add_control(const ControlSettings& settings)
  {
    add_control(Control(settings));
  }

  void Action::add_control(const Control& control)
  {
    m_controls.push_back(control);
  }

  bool Action::active() const
  {
    return std::any_of(m_controls.begin(), m_controls.end(), std::mem_fn(&Control::active));
  }

  void Action::reset()
  {
    if (continuous()) {
      return;
    }

    for (auto& control : m_controls) {
      control.reset();
    }
  }

  void Action::process_event(const Event& event)
  {
    for (auto& control : m_controls) {
      control.process_event(event);
    }
  }

}
