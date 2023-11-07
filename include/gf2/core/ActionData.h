// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_ACTION_DATA_H
#define GF_ACTION_DATA_H

#include <cstdint>

#include <map>
#include <vector>

#include "ControlData.h"
#include "CoreApi.h"
#include "Id.h"
#include "TypeTraits.h"

namespace gf {

  enum class ActionType : uint8_t {
    Instantaneous,
    Continuous,
  };

  struct GF_CORE_API ActionData {
    ActionData() = default;

    ActionData(ActionType type)
    : type(type)
    {
    }

    ActionData& add_keycode_control(Keycode keycode, Flags<Modifier> modifiers = None)
    {
      controls.emplace_back(keycode, modifiers);
      return *this;
    }

    ActionData& add_scancode_control(Scancode scancode, Flags<Modifier> modifiers = None)
    {
      controls.emplace_back(scancode, modifiers);
      return *this;
    }

    ActionData& add_mouse_button_control(MouseButton button)
    {
      controls.emplace_back(button);
      return *this;
    }

    ActionData& add_gamepad_button_control(GamepadId gamepad_id, GamepadButton button)
    {
      controls.emplace_back(gamepad_id, button);
      return *this;
    }

    ActionData& add_gamepad_axis_control(GamepadId gamepad_id, GamepadAxis axis, GamepadAxisDirection direction)
    {
      controls.emplace_back(gamepad_id, axis, direction);
      return *this;
    }

    ActionType type = ActionType::Instantaneous;
    std::vector<ControlData> controls;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ActionData, Archive>& data)
  {
    return ar | data.type | data.controls;
  }

  struct GF_CORE_API ActionGroupData {
    std::map<Id, ActionData> actions;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ActionGroupData, Archive>& data)
  {
    return ar | data.actions;
  }

}

#endif // GF_ACTION_DATA_H
