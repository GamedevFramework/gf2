// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CORE_GAMEPAD_TYPES_H
#define GF_CORE_GAMEPAD_TYPES_H

#include <cstdint>

#include <limits>
#include <type_traits>

namespace gf {

  enum class GamepadButton : int32_t { // NOLINT(performance-enum-size)
    Invalid = -1,
    South,
    East,
    West,
    North,
    Back,
    Guide,
    Start,
    LeftStick,
    RightStick,
    LeftBumper,
    RightBumper,
    DPadUp,
    DPadDown,
    DPadLeft,
    DPadRight,
  };

  constexpr GamepadButton AnyGamepadButton = static_cast<GamepadButton>(0x7FFFFFFF);

  enum class GamepadStick : uint8_t {
    Left,
    Right,
  };

  enum class GamepadAxis : int32_t { // NOLINT(performance-enum-size)
    Invalid = -1,
    LeftX,
    LeftY,
    RightX,
    RightY,
    TriggerLeft,
    TriggerRight,
  };

  constexpr GamepadAxis AnyGamepadAxis = static_cast<GamepadAxis>(0x7FFFFFFF);

  enum class GamepadAxisDirection : uint8_t {
    Positive,
    Negative,
  };

  enum class GamepadId : uint32_t { // NOLINT(performance-enum-size)
  };

  constexpr GamepadId AnyGamepad = static_cast<GamepadId>(std::numeric_limits<std::underlying_type_t<GamepadId>>::max());

}

#endif // GF_CORE_GAMEPAD_TYPES_H
