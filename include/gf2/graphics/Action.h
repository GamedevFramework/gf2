// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_ACTION_H
#define GF_ACTION_H

#include <vector>

#include <gf2/core/ActionSettings.h>

#include "Control.h"
#include "GraphicsApi.h"

namespace gf {

  class GF_GRAPHICS_API Action {
  public:
    Action(ActionType type = ActionType::Instantaneous);
    Action(const ActionSettings& settings);

    void set_continuous();
    bool continuous() const;

    void set_instantaneous();
    bool instantaneous() const;

    void add_keycode_control(Keycode keycode, Flags<Modifier> modifiers = None);
    void add_scancode_control(Scancode scancode, Flags<Modifier> modifiers = None);
    void add_mouse_button_control(MouseButton button);
    void add_gamepad_button_control(GamepadId gamepad_id, GamepadButton button);
    void add_gamepad_axis_control(GamepadId gamepad_id, GamepadAxis axis, GamepadAxisDirection direction);

    void add_control(const Control& control);
    void add_control_settings(const ControlSettings& settings);

    const Control* begin() const
    {
      return m_controls.data();
    }

    const Control* end() const
    {
      return m_controls.data() + m_controls.size();
    }

    bool active() const;

    void reset();
    void hard_reset();

    void process_event(const Event& event);

  private:
    ActionType m_type = ActionType::Instantaneous;
    std::vector<Control> m_controls;
  };

}

#endif // GF_ACTION_H
