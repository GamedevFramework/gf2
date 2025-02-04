// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Gamepad.h>

#include <SDL3/SDL.h>

#include <gf2/core/Log.h>

namespace gf {

  namespace {

    template<GamepadButton Button, SDL_GamepadButton Value>
    constexpr void gamepad_button_check()
    {
      static_assert(static_cast<SDL_GamepadButton>(Button) == Value);
    }

    template<GamepadAxis Axis, SDL_GamepadAxis Value>
    constexpr void gamepad_axis_check()
    {
      static_assert(static_cast<SDL_GamepadAxis>(Axis) == Value, "Check gf::GamepadAxis");
    }

    [[maybe_unused]] constexpr void gamepad_check()
    {
      gamepad_button_check<GamepadButton::Invalid, SDL_GAMEPAD_BUTTON_INVALID>();
      gamepad_button_check<GamepadButton::South, SDL_GAMEPAD_BUTTON_SOUTH>();
      gamepad_button_check<GamepadButton::East, SDL_GAMEPAD_BUTTON_EAST>();
      gamepad_button_check<GamepadButton::West, SDL_GAMEPAD_BUTTON_WEST>();
      gamepad_button_check<GamepadButton::North, SDL_GAMEPAD_BUTTON_NORTH>();
      gamepad_button_check<GamepadButton::Back, SDL_GAMEPAD_BUTTON_BACK>();
      gamepad_button_check<GamepadButton::Guide, SDL_GAMEPAD_BUTTON_GUIDE>();
      gamepad_button_check<GamepadButton::Start, SDL_GAMEPAD_BUTTON_START>();
      gamepad_button_check<GamepadButton::LeftStick, SDL_GAMEPAD_BUTTON_LEFT_STICK>();
      gamepad_button_check<GamepadButton::RightStick, SDL_GAMEPAD_BUTTON_RIGHT_STICK>();
      gamepad_button_check<GamepadButton::LeftBumper, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER>();
      gamepad_button_check<GamepadButton::RightBumper, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER>();
      gamepad_button_check<GamepadButton::DPadUp, SDL_GAMEPAD_BUTTON_DPAD_UP>();
      gamepad_button_check<GamepadButton::DPadDown, SDL_GAMEPAD_BUTTON_DPAD_DOWN>();
      gamepad_button_check<GamepadButton::DPadLeft, SDL_GAMEPAD_BUTTON_DPAD_LEFT>();
      gamepad_button_check<GamepadButton::DPadRight, SDL_GAMEPAD_BUTTON_DPAD_RIGHT>();

      gamepad_axis_check<GamepadAxis::Invalid, SDL_GAMEPAD_AXIS_INVALID>();
      gamepad_axis_check<GamepadAxis::LeftX, SDL_GAMEPAD_AXIS_LEFTX>();
      gamepad_axis_check<GamepadAxis::LeftY, SDL_GAMEPAD_AXIS_LEFTY>();
      gamepad_axis_check<GamepadAxis::RightX, SDL_GAMEPAD_AXIS_RIGHTX>();
      gamepad_axis_check<GamepadAxis::RightY, SDL_GAMEPAD_AXIS_RIGHTY>();
      gamepad_axis_check<GamepadAxis::TriggerLeft, SDL_GAMEPAD_AXIS_LEFT_TRIGGER>();
      gamepad_axis_check<GamepadAxis::TriggerRight, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER>();
    }

  } // namespace

  const char* Gamepad::axis_name(GamepadAxis axis)
  {
    return SDL_GetGamepadStringForAxis(static_cast<SDL_GamepadAxis>(axis));
  }

  const char* Gamepad::button_name(GamepadButton button)
  {
    return SDL_GetGamepadStringForButton(static_cast<SDL_GamepadButton>(button));
  }

  const char* Gamepad::gamepad_name(GamepadId id)
  {
    SDL_Gamepad* controller = SDL_GetGamepadFromID(static_cast<SDL_JoystickID>(id));

    if (controller == nullptr) {
      return "?";
    }

    return SDL_GetGamepadName(controller);
  }

  void Gamepad::open(GamepadId hwid)
  {
    const int index = static_cast<int>(hwid);
    SDL_Gamepad* controller = SDL_OpenGamepad(index);

    if (controller == nullptr) {
      Log::error("Could not open gamepad {}: {}", index, SDL_GetError());
      return;
    }

    // TODO: [SDL3] do better
    // SDL_Joystick* joystick = SDL_GameControllerGetJoystick(controller);
    // const SDL_JoystickID instance_id = SDL_JoystickInstanceID(joystick);
    //
    // Log::debug("New gamepad (device: {} / instance: {}): '{}'", index, instance_id, SDL_GameControllerName(controller));
  }

  bool Gamepad::attached(GamepadId id)
  {
    SDL_Gamepad* controller = SDL_GetGamepadFromID(static_cast<SDL_JoystickID>(id));

    if (controller == nullptr) {
      return false;
    }

    return SDL_GamepadConnected(controller);
  }

  void Gamepad::close(GamepadId id)
  {
    SDL_Gamepad* controller = SDL_GetGamepadFromID(static_cast<SDL_JoystickID>(id));

    if (controller == nullptr) {
      return;
    }

    SDL_CloseGamepad(controller);
  }

} // namespace gf
