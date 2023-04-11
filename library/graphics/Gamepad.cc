// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Gamepad.h>
// clang-format on

#include <SDL2/SDL.h>
#include <gf2/Log.h>

namespace gf {

  namespace {

    template<GamepadButton Button, SDL_GameControllerButton Value>
    constexpr void gamepad_button_check()
    {
      static_assert(static_cast<SDL_GameControllerButton>(Button) == Value, "Check gf::GamepadButton");
    }

    template<GamepadAxis Axis, SDL_GameControllerAxis Value>
    constexpr void gamepad_axis_check()
    {
      static_assert(static_cast<SDL_GameControllerAxis>(Axis) == Value, "Check gf::GamepadAxis");
    }

    [[maybe_unused]] constexpr void gamepad_check()
    {
      gamepad_button_check<GamepadButton::Invalid, SDL_CONTROLLER_BUTTON_INVALID>();
      gamepad_button_check<GamepadButton::A, SDL_CONTROLLER_BUTTON_A>();
      gamepad_button_check<GamepadButton::B, SDL_CONTROLLER_BUTTON_B>();
      gamepad_button_check<GamepadButton::X, SDL_CONTROLLER_BUTTON_X>();
      gamepad_button_check<GamepadButton::Y, SDL_CONTROLLER_BUTTON_Y>();
      gamepad_button_check<GamepadButton::Back, SDL_CONTROLLER_BUTTON_BACK>();
      gamepad_button_check<GamepadButton::Guide, SDL_CONTROLLER_BUTTON_GUIDE>();
      gamepad_button_check<GamepadButton::Start, SDL_CONTROLLER_BUTTON_START>();
      gamepad_button_check<GamepadButton::LeftStick, SDL_CONTROLLER_BUTTON_LEFTSTICK>();
      gamepad_button_check<GamepadButton::RightStick, SDL_CONTROLLER_BUTTON_RIGHTSTICK>();
      gamepad_button_check<GamepadButton::LeftBumper, SDL_CONTROLLER_BUTTON_LEFTSHOULDER>();
      gamepad_button_check<GamepadButton::RightBumper, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER>();
      gamepad_button_check<GamepadButton::DPadUp, SDL_CONTROLLER_BUTTON_DPAD_UP>();
      gamepad_button_check<GamepadButton::DPadDown, SDL_CONTROLLER_BUTTON_DPAD_DOWN>();
      gamepad_button_check<GamepadButton::DPadLeft, SDL_CONTROLLER_BUTTON_DPAD_LEFT>();
      gamepad_button_check<GamepadButton::DPadRight, SDL_CONTROLLER_BUTTON_DPAD_RIGHT>();

      gamepad_axis_check<GamepadAxis::Invalid, SDL_CONTROLLER_AXIS_INVALID>();
      gamepad_axis_check<GamepadAxis::LeftX, SDL_CONTROLLER_AXIS_LEFTX>();
      gamepad_axis_check<GamepadAxis::LeftY, SDL_CONTROLLER_AXIS_LEFTY>();
      gamepad_axis_check<GamepadAxis::RightX, SDL_CONTROLLER_AXIS_RIGHTX>();
      gamepad_axis_check<GamepadAxis::RightY, SDL_CONTROLLER_AXIS_RIGHTY>();
      gamepad_axis_check<GamepadAxis::TriggerLeft, SDL_CONTROLLER_AXIS_TRIGGERLEFT>();
      gamepad_axis_check<GamepadAxis::TriggerRight, SDL_CONTROLLER_AXIS_TRIGGERRIGHT>();
    }

  } // namespace

  const char* Gamepad::axis_name(GamepadAxis axis)
  {
    return SDL_GameControllerGetStringForAxis(static_cast<SDL_GameControllerAxis>(axis));
  }

  const char* Gamepad::button_name(GamepadButton button)
  {
    return SDL_GameControllerGetStringForButton(static_cast<SDL_GameControllerButton>(button));
  }

  const char* Gamepad::gamepad_name(GamepadId id)
  {
    SDL_GameController* controller = SDL_GameControllerFromInstanceID(static_cast<SDL_JoystickID>(id));

    if (controller == nullptr) {
      return "?";
    }

    return SDL_GameControllerName(controller);
  }

  GamepadId Gamepad::open(GamepadHwId hwid)
  {
    const int index = static_cast<int>(hwid);
    SDL_GameController* controller = SDL_GameControllerOpen(index);

    if (controller == nullptr) {
      Log::error("Could not open gamepad {}: {}", index, SDL_GetError());
      return static_cast<GamepadId>(-1);
    }

    SDL_Joystick* joystick = SDL_GameControllerGetJoystick(controller);
    const SDL_JoystickID instance_id = SDL_JoystickInstanceID(joystick);

    Log::debug("New gamepad (device: {} / instance: {}): '{}'", index, instance_id, SDL_GameControllerName(controller));
    return static_cast<GamepadId>(instance_id);
  }

  bool Gamepad::attached(GamepadId id)
  {
    SDL_GameController* controller = SDL_GameControllerFromInstanceID(static_cast<SDL_JoystickID>(id));

    if (controller == nullptr) {
      return false;
    }

    return SDL_GameControllerGetAttached(controller) == SDL_TRUE;
  }

  void Gamepad::close(GamepadId id)
  {
    SDL_GameController* controller = SDL_GameControllerFromInstanceID(static_cast<SDL_JoystickID>(id));

    if (controller == nullptr) {
      return;
    }

    SDL_GameControllerClose(controller);
  }

} // namespace gf
