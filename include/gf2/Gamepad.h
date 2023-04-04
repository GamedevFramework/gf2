// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_GAMEPAD_H
#define GF_GAMEPAD_H

#include <cstdint>

#include "GraphicsApi.h"

namespace gf {

  enum class GamepadButton : int32_t {
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

  enum class GamepadAxis : int32_t {
    Invalid = -1,
    LeftX,
    LeftY,
    RightX,
    RightY,
    TriggerLeft,
    TriggerRight,
  };

  enum class GamepadAxisDirection : uint32_t {
    Positive,
    Negative,
  };

  enum class GamepadHwId : int {
  };

  enum class GamepadId : int32_t {
  };

  constexpr GamepadId AnyGamepad = static_cast<GamepadId>(INT32_C(-1));

  class GF_GRAPHICS_API Gamepad {
  public:
    static const char* axis_name(GamepadAxis axis);
    static const char* button_name(GamepadButton button);
    static const char* gamepad_name(GamepadId id);

    static GamepadId open(GamepadHwId hwid);
    static bool attached(GamepadId id);
    static void close(GamepadId id);
  };

} // namespace gf

#endif // GF_GAMEPAD_H
