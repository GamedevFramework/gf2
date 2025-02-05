// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CORE_GAMEPAD_TYPES_H
#define GF_CORE_GAMEPAD_TYPES_H

#include <cstdint>

#include <limits>
#include <type_traits>

namespace gf {

  enum class GamepadButton { // NOLINT(performance-enum-size)
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

  constexpr GamepadButton AnyGamepadButton = GamepadButton{ std::numeric_limits<std::underlying_type_t<GamepadButton>>::max() };

  enum class GamepadStick : uint8_t {
    Left,
    Right,
  };

  enum class GamepadAxis { // NOLINT(performance-enum-size)
    Invalid = -1,
    LeftX,
    LeftY,
    RightX,
    RightY,
    TriggerLeft,
    TriggerRight,
  };

  constexpr GamepadAxis AnyGamepadAxis = GamepadAxis{ std::numeric_limits<std::underlying_type_t<GamepadAxis>>::max() };

  enum class GamepadAxisDirection : uint8_t {
    Positive,
    Negative,
  };

  enum class GamepadId : uint32_t { // NOLINT(performance-enum-size)
  };

  constexpr GamepadId AnyGamepad = static_cast<GamepadId>(std::numeric_limits<std::underlying_type_t<GamepadId>>::max());

  enum class GamepadType { // NOLINT(performance-enum-size)
    Unknown,
    Standard,
    XBox360,
    XBoxOne,
    Ps3,
    Ps4,
    Ps5,
    NintendoSwitchPro,
    NintendoSwitchJoyconLeft,
    NintendoSwitchJoyconRight,
    NintendoSwitchJoyconPair,
  };

  enum class GamepadButtonLabel { // NOLINT(performance-enum-size)
    Unknown,
    A,
    B,
    X,
    Y,
    Cross,
    Circle,
    Square,
    Triangle,
  };

  enum class GamepadConnectionState { // NOLINT(performance-enum-size)
    Invalid = -1,
    Unknown,
    Wired,
    Wireless,
  };

}

#endif // GF_CORE_GAMEPAD_TYPES_H
