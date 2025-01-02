// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_ACTION_DATA_H
#define GF_ACTION_DATA_H

#include <cstdint>

#include <map>
#include <vector>

#include "ControlSettings.h"
#include "CoreApi.h"
#include "Id.h"
#include "TypeTraits.h"

namespace gf {

  enum class ActionType : uint8_t {
    Instantaneous,
    Continuous,
  };

  struct GF_CORE_API ActionSettings {
    ActionSettings() = default;

    ActionSettings(ActionType type)
    : type(type)
    {
    }

    ActionSettings& add_keycode_control(Keycode keycode, Flags<Modifier> modifiers = None)
    {
      controls.emplace_back(keycode, modifiers);
      return *this;
    }

    ActionSettings& add_scancode_control(Scancode scancode, Flags<Modifier> modifiers = None)
    {
      controls.emplace_back(scancode, modifiers);
      return *this;
    }

    ActionSettings& add_mouse_button_control(MouseButton button)
    {
      controls.emplace_back(button);
      return *this;
    }

    ActionSettings& add_gamepad_button_control(GamepadId gamepad_id, GamepadButton button)
    {
      controls.emplace_back(gamepad_id, button);
      return *this;
    }

    ActionSettings& add_gamepad_axis_control(GamepadId gamepad_id, GamepadAxis axis, GamepadAxisDirection direction)
    {
      controls.emplace_back(gamepad_id, axis, direction);
      return *this;
    }

    ActionType type = ActionType::Instantaneous;
    std::vector<ControlSettings> controls;
  };

  inline ActionSettings instantaneous_action()
  {
    return { ActionType::Instantaneous };
  }

  inline ActionSettings continuous_action()
  {
    return { ActionType::Continuous };
  }

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ActionSettings, Archive>& settings)
  {
    return ar | settings.type | settings.controls;
  }

  struct GF_CORE_API ActionGroupSettings {
    std::map<Id, ActionSettings> actions;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ActionGroupSettings, Archive>& settings)
  {
    return ar | settings.actions;
  }

}

#endif // GF_ACTION_DATA_H
