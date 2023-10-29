// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CORE_GAMEPAD_TYPES_H
#define GF_CORE_GAMEPAD_TYPES_H

#include <cstdint>

namespace gf {

  enum class GamepadButton : int32_t { // NOLINT(performance-enum-size)
    Invalid = -1,
    A,
    B,
    X,
    Y,
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

  enum class GamepadHwId : int { // NOLINT(performance-enum-size)
  };

  enum class GamepadId : int32_t { // NOLINT(performance-enum-size)
  };

  constexpr GamepadId AnyGamepad = static_cast<GamepadId>(INT32_C(-1));

}

#endif // GF_CORE_GAMEPAD_TYPES_H
