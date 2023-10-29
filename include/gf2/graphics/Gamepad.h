// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_GAMEPAD_H
#define GF_GAMEPAD_H

#include <gf2/core/GamepadTypes.h>

#include "GraphicsApi.h"

namespace gf {

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
