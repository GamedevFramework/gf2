// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CONTROL_H
#define GF_CONTROL_H

#include <gf2/core/ControlData.h>

#include "Event.h"
#include "GraphicsApi.h"

namespace gf {

  class GF_GRAPHICS_API Control {
  public:
    Control(const ControlData& data);

    bool active() const;
    void trigger();
    void reset();

    void process_event(const Event& event);

    ControlData data() const
    {
      return m_data;
    }

  private:
    void process_keycode_control(const Event& event, const KeycodeControlData& data);
    void process_scancode_control(const Event& event, const ScancodeControlData& data);
    void process_mouse_button_control(const Event& event, const MouseButtonControlData& data);
    void process_gamepad_button_control(const Event& event, const GamepadButtonControlData& data);
    void process_gamepad_axis_control(const Event& event, const GamepadAxisControlData& data);

    ControlData m_data = {};
    bool m_active = false;
    bool m_repeated = false;
  };

}

#endif // GF_CONTROL_H
