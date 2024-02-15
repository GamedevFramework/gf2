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

  Action::Action(const ActionData& data)
  : m_type(data.type)
  {
    for (const auto& control : data.controls) {
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
    ControlData data = {};
    data.type = ControlType::Keycode;
    data.keycode.keycode = keycode;
    data.keycode.modifiers = modifiers;
    add_control(data);
  }

  void Action::add_scancode_control(Scancode scancode, Flags<Modifier> modifiers)
  {
    ControlData data = {};
    data.type = ControlType::Scancode;
    data.scancode.scancode = scancode;
    data.scancode.modifiers = modifiers;
    add_control(data);
  }

  void Action::add_mouse_button_control(MouseButton button)
  {
    ControlData data = {};
    data.type = ControlType::MouseButton;
    data.mouse_button.button = button;
    add_control(data);
  }

  void Action::add_gamepad_button_control(GamepadId gamepad_id, GamepadButton button)
  {
    ControlData data = {};
    data.type = ControlType::GamepadButton;
    data.gamepad_button.gamepad_id = gamepad_id;
    data.gamepad_button.button = button;
    add_control(data);
  }

  void Action::add_gamepad_axis_control(GamepadId gamepad_id, GamepadAxis axis, GamepadAxisDirection direction)
  {
    ControlData data = {};
    data.type = ControlType::GamepadAxis;
    data.gamepad_axis.gamepad_id = gamepad_id;
    data.gamepad_axis.axis = axis;
    data.gamepad_axis.direction = direction;
    add_control(data);
  }

  void Action::add_control(const ControlData& data)
  {
    add_control(Control(data));
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
