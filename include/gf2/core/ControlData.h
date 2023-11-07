// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CONTROL_DATA_H
#define GF_CONTROL_DATA_H

#include <cstdint>

#include "CoreApi.h"
#include "GamepadTypes.h"
#include "Keycode.h"
#include "Modifier.h"
#include "MouseTypes.h"
#include "Scancode.h"
#include "TypeTraits.h"

namespace gf {

  struct GF_CORE_API KeycodeControlData {
    Keycode keycode;
    Flags<Modifier> modifiers;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<KeycodeControlData, Archive>& data)
  {
    return ar | data.keycode | data.modifiers;
  }

  struct GF_CORE_API ScancodeControlData {
    Scancode scancode;
    Flags<Modifier> modifiers;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ScancodeControlData, Archive>& data)
  {
    return ar | data.scancode | data.modifiers;
  }

  struct GF_CORE_API MouseButtonControlData {
    MouseButton button;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<MouseButtonControlData, Archive>& data)
  {
    return ar | data.button;
  }

  struct GF_CORE_API GamepadButtonControlData {
    GamepadId gamepad_id;
    GamepadButton button;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<GamepadButtonControlData, Archive>& data)
  {
    return ar | data.gamepad_id | data.button;
  }

  struct GF_CORE_API GamepadAxisControlData {
    GamepadId gamepad_id;
    GamepadAxis axis;
    GamepadAxisDirection direction;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<GamepadAxisControlData, Archive>& data)
  {
    return ar | data.gamepad_id | data.axis | data.direction;
  }

  enum class ControlType : uint8_t {
    Keycode,
    Scancode,
    MouseButton,
    GamepadButton,
    GamepadAxis,
  };

  struct GF_CORE_API ControlData {
    ControlData() = default;

    constexpr ControlData(Keycode keycode, Flags<Modifier> modifiers = None)
    : type(ControlType::Keycode)
    , keycode({ keycode, modifiers })
    {
    }

    constexpr ControlData(Scancode scancode, Flags<Modifier> modifiers = None)
    : type(ControlType::Scancode)
    , scancode({ scancode, modifiers })
    {
    }

    constexpr ControlData(MouseButton button)
    : type(ControlType::MouseButton)
    , mouse_button({ button })
    {
    }

    constexpr ControlData(GamepadId gamepad_id, GamepadButton button)
    : type(ControlType::GamepadButton)
    , gamepad_button({ gamepad_id, button })
    {
    }

    constexpr ControlData(GamepadId gamepad_id, GamepadAxis axis, GamepadAxisDirection direction)
    : type(ControlType::GamepadAxis)
    , gamepad_axis({ gamepad_id, axis, direction })
    {
    }

    ControlType type;
    union {
      KeycodeControlData keycode;
      ScancodeControlData scancode;
      MouseButtonControlData mouse_button;
      GamepadButtonControlData gamepad_button;
      GamepadAxisControlData gamepad_axis;
    };
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ControlData, Archive>& data)
  {
    ar | data.type;

    switch (data.type) {
      case ControlType::Keycode:
        ar | data.keycode;
        break;
      case ControlType::Scancode:
        ar | data.scancode;
        break;
      case ControlType::MouseButton:
        ar | data.mouse_button;
        break;
      case ControlType::GamepadButton:
        ar | data.gamepad_button;
        break;
      case ControlType::GamepadAxis:
        ar | data.gamepad_axis;
        break;
    }

    return ar;
  }

}

#endif // GF_CONTROL_DATA_H
