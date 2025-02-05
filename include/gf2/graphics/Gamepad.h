// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GAMEPAD_H
#define GF_GAMEPAD_H

#include <cstdint>

#include <gf2/core/GamepadTypes.h>
#include <gf2/core/Id.h>
#include <gf2/core/ZString.h>

#include "GraphicsApi.h"

using SDL_Gamepad = struct SDL_Gamepad;

namespace gf {

  class GF_GRAPHICS_API GamepadDevice {
  public:
    operator bool()
    {
      return m_gamepad != nullptr;
    }

    bool connected();
    GamepadId id();

    GamepadType type();
    GamepadType real_type();

    bool has_axis(GamepadAxis axis);
    bool has_button(GamepadButton button);

    GamepadButtonLabel button_label(GamepadButton button);
    GamepadConnectionState connection_state();

    uint16_t vendor();
    uint16_t product();
    uint16_t product_version();
    uint16_t firmware_version();

    ZString name();
    ZString path();
    ZString serial();
    Id serial_id();

    void close();

  private:
    friend struct Gamepad;
    GamepadDevice(SDL_Gamepad* gamepad);
    SDL_Gamepad* m_gamepad = nullptr;
  };

  struct GF_GRAPHICS_API Gamepad {
    static GamepadDevice open(GamepadId id);
    static GamepadDevice from_id(GamepadId id);

    static ZString name(GamepadId id);
    static ZString path(GamepadId id);
    static GamepadType type(GamepadId id);
    static GamepadType real_type(GamepadId id);
    static uint16_t vendor(GamepadId id);
    static uint16_t product(GamepadId id);
    static uint16_t product_version(GamepadId id);

    static ZString axis_name(GamepadAxis axis);
    static ZString button_name(GamepadButton button);
    static ZString type_name(GamepadType type);

    static GamepadButtonLabel button_label_for_type(GamepadType type, GamepadButton button);
  };

} // namespace gf

#endif // GF_GAMEPAD_H
