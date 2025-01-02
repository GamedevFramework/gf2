// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONTROL_H
#define GF_CONTROL_H

#include <gf2/core/ControlSettings.h>

#include "Event.h"
#include "GraphicsApi.h"

namespace gf {

  class GF_GRAPHICS_API Control {
  public:
    Control(const ControlSettings& settings);

    bool active() const;
    void trigger();
    void reset();

    void process_event(const Event& event);

    ControlSettings settings() const
    {
      return m_settings;
    }

  private:
    void process_keycode_control(const Event& event, const KeycodeControlSettings& settings);
    void process_scancode_control(const Event& event, const ScancodeControlSettings& settings);
    void process_mouse_button_control(const Event& event, const MouseButtonControlSettings& settings);
    void process_gamepad_button_control(const Event& event, const GamepadButtonControlSettings& settings);
    void process_gamepad_axis_control(const Event& event, const GamepadAxisControlSettings& settings);

    ControlSettings m_settings;
    bool m_active = false;
    bool m_repeated = false;
  };

}

#endif // GF_CONTROL_H
