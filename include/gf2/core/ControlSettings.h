// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CONTROL_DATA_H
#define GF_CONTROL_DATA_H

#include <cstdint>

#include <variant>

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
    None,
    Keycode,
    Scancode,
    MouseButton,
    GamepadButton,
    GamepadAxis,
  };

  class GF_CORE_API ControlSettings {
  public:
    ControlSettings() = default;

    constexpr ControlSettings(Keycode keycode, Flags<Modifier> modifiers = None)
    : m_data(KeycodeControlSettings{ keycode, modifiers })
    {
    }

    constexpr ControlSettings(Scancode scancode, Flags<Modifier> modifiers = None)
    : m_data(ScancodeControlSettings{ scancode, modifiers })
    {
    }

    constexpr ControlSettings(MouseButton button)
    : m_data(MouseButtonControlSettings{ button })
    {
    }

    constexpr ControlSettings(GamepadId gamepad_id, GamepadButton button)
    : m_data(GamepadButtonControlSettings{ gamepad_id, button })
    {
    }

    constexpr ControlSettings(GamepadId gamepad_id, GamepadAxis axis, GamepadAxisDirection direction)
    : m_data(GamepadAxisControlSettings{ gamepad_id, axis, direction })
    {
    }

    constexpr ControlType type() const
    {
      return static_cast<ControlType>(m_data.index());
    }

    template<typename T>
    constexpr T& as()
    {
      return std::get<T>(m_data);
    }

    template<typename T>
    constexpr const T& as() const
    {
      return std::get<T>(m_data);
    }

  private:
    std::variant<
      std::monostate,
      KeycodeControlSettings,
      ScancodeControlSettings,
      MouseButtonControlSettings,
      GamepadButtonControlSettings,
      GamepadAxisControlSettings
    > m_data;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ControlSettings, Archive>& settings)
  {
    ControlType type = settings.type();
    ar | type;

    switch (type) {
      case ControlType::None:
        break;
      case ControlType::Keycode:
        ar | settings.template as<KeycodeControlSettings>();
        break;
      case ControlType::Scancode:
        ar | settings.template as<ScancodeControlSettings>();
        break;
      case ControlType::MouseButton:
        ar | settings.template as<MouseButtonControlSettings>();
        break;
      case ControlType::GamepadButton:
        ar | settings.template as<GamepadButtonControlSettings>();
        break;
      case ControlType::GamepadAxis:
        ar | settings.template as<GamepadAxisControlSettings>();
        break;
    }

    return ar;
  }

}

#endif // GF_CONTROL_DATA_H
