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

  struct GF_CORE_API KeycodeControlSettings {
    Keycode keycode;
    Flags<Modifier> modifiers;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<KeycodeControlSettings, Archive>& settings)
  {
    return ar | settings.keycode | settings.modifiers;
  }

  struct GF_CORE_API ScancodeControlSettings {
    Scancode scancode;
    Flags<Modifier> modifiers;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ScancodeControlSettings, Archive>& settings)
  {
    return ar | settings.scancode | settings.modifiers;
  }

  struct GF_CORE_API MouseButtonControlSettings {
    MouseButton button;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<MouseButtonControlSettings, Archive>& settings)
  {
    return ar | settings.button;
  }

  struct GF_CORE_API GamepadButtonControlSettings {
    GamepadId gamepad_id;
    GamepadButton button;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<GamepadButtonControlSettings, Archive>& settings)
  {
    return ar | settings.gamepad_id | settings.button;
  }

  struct GF_CORE_API GamepadAxisControlSettings {
    GamepadId gamepad_id;
    GamepadAxis axis;
    GamepadAxisDirection direction;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<GamepadAxisControlSettings, Archive>& settings)
  {
    return ar | settings.gamepad_id | settings.axis | settings.direction;
  }

  enum class ControlType : uint8_t {
    Keycode,
    Scancode,
    MouseButton,
    GamepadButton,
    GamepadAxis,
  };

  struct GF_CORE_API ControlSettings {
    ControlSettings() = default;

    constexpr ControlSettings(Keycode keycode, Flags<Modifier> modifiers = None)
    : type(ControlType::Keycode)
    , keycode({ keycode, modifiers })
    {
    }

    constexpr ControlSettings(Scancode scancode, Flags<Modifier> modifiers = None)
    : type(ControlType::Scancode)
    , scancode({ scancode, modifiers })
    {
    }

    constexpr ControlSettings(MouseButton button)
    : type(ControlType::MouseButton)
    , mouse_button({ button })
    {
    }

    constexpr ControlSettings(GamepadId gamepad_id, GamepadButton button)
    : type(ControlType::GamepadButton)
    , gamepad_button({ gamepad_id, button })
    {
    }

    constexpr ControlSettings(GamepadId gamepad_id, GamepadAxis axis, GamepadAxisDirection direction)
    : type(ControlType::GamepadAxis)
    , gamepad_axis({ gamepad_id, axis, direction })
    {
    }

    ControlType type;
    union {
      KeycodeControlSettings keycode;
      ScancodeControlSettings scancode;
      MouseButtonControlSettings mouse_button;
      GamepadButtonControlSettings gamepad_button;
      GamepadAxisControlSettings gamepad_axis;
    };
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ControlSettings, Archive>& settings)
  {
    ar | settings.type;

    switch (settings.type) {
      case ControlType::Keycode:
        ar | settings.keycode;
        break;
      case ControlType::Scancode:
        ar | settings.scancode;
        break;
      case ControlType::MouseButton:
        ar | settings.mouse_button;
        break;
      case ControlType::GamepadButton:
        ar | settings.gamepad_button;
        break;
      case ControlType::GamepadAxis:
        ar | settings.gamepad_axis;
        break;
    }

    return ar;
  }

}

#endif // GF_CONTROL_DATA_H
