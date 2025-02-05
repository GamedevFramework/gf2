// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Gamepad.h>

#include <type_traits>

#include <SDL3/SDL.h>

#include <gf2/core/Log.h>

namespace gf {

  namespace {

    static_assert(std::is_same_v<std::underlying_type_t<GamepadButton>, std::underlying_type_t<SDL_GamepadButton>>);
    static_assert(std::is_same_v<std::underlying_type_t<GamepadAxis>, std::underlying_type_t<SDL_GamepadAxis>>);
    static_assert(std::is_same_v<std::underlying_type_t<GamepadId>, SDL_JoystickID>);
    static_assert(std::is_same_v<std::underlying_type_t<GamepadType>, std::make_signed_t<SDL_GamepadType>>);
    static_assert(std::is_same_v<std::underlying_type_t<GamepadButtonLabel>, std::make_signed_t<SDL_GamepadButtonLabel>>);
    static_assert(std::is_same_v<std::underlying_type_t<GamepadConnectionState>, std::underlying_type_t<SDL_JoystickConnectionState>>);

    template<GamepadButton Button, SDL_GamepadButton Value>
    constexpr void gamepad_button_check()
    {
      static_assert(static_cast<SDL_GamepadButton>(Button) == Value);
    }

    template<GamepadAxis Axis, SDL_GamepadAxis Value>
    constexpr void gamepad_axis_check()
    {
      static_assert(static_cast<SDL_GamepadAxis>(Axis) == Value);
    }

    template<GamepadType Type, SDL_GamepadType Value>
    constexpr void gamepad_type_check()
    {
      static_assert(static_cast<SDL_GamepadType>(Type) == Value);
    }

    template<GamepadButtonLabel Label, SDL_GamepadButtonLabel Value>
    constexpr void gamepad_button_label_check()
    {
      static_assert(static_cast<SDL_GamepadButtonLabel>(Label) == Value);
    }

    template<GamepadConnectionState State, SDL_JoystickConnectionState Value>
    constexpr void gamepad_connection_state_check()
    {
      static_assert(static_cast<SDL_JoystickConnectionState>(State) == Value);
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

      gamepad_type_check<GamepadType::Unknown, SDL_GAMEPAD_TYPE_UNKNOWN>();
      gamepad_type_check<GamepadType::Standard, SDL_GAMEPAD_TYPE_STANDARD>();
      gamepad_type_check<GamepadType::XBox360, SDL_GAMEPAD_TYPE_XBOX360>();
      gamepad_type_check<GamepadType::XBoxOne, SDL_GAMEPAD_TYPE_XBOXONE>();
      gamepad_type_check<GamepadType::Ps3, SDL_GAMEPAD_TYPE_PS3>();
      gamepad_type_check<GamepadType::Ps4, SDL_GAMEPAD_TYPE_PS4>();
      gamepad_type_check<GamepadType::Ps5, SDL_GAMEPAD_TYPE_PS5>();
      gamepad_type_check<GamepadType::NintendoSwitchPro, SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO>();
      gamepad_type_check<GamepadType::NintendoSwitchJoyconLeft, SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT>();
      gamepad_type_check<GamepadType::NintendoSwitchJoyconRight, SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT>();
      gamepad_type_check<GamepadType::NintendoSwitchJoyconPair, SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR>();

      gamepad_button_label_check<GamepadButtonLabel::Unknown, SDL_GAMEPAD_BUTTON_LABEL_UNKNOWN>();
      gamepad_button_label_check<GamepadButtonLabel::A, SDL_GAMEPAD_BUTTON_LABEL_A>();
      gamepad_button_label_check<GamepadButtonLabel::B, SDL_GAMEPAD_BUTTON_LABEL_B>();
      gamepad_button_label_check<GamepadButtonLabel::X, SDL_GAMEPAD_BUTTON_LABEL_X>();
      gamepad_button_label_check<GamepadButtonLabel::Y, SDL_GAMEPAD_BUTTON_LABEL_Y>();
      gamepad_button_label_check<GamepadButtonLabel::Cross, SDL_GAMEPAD_BUTTON_LABEL_CROSS>();
      gamepad_button_label_check<GamepadButtonLabel::Circle, SDL_GAMEPAD_BUTTON_LABEL_CIRCLE>();
      gamepad_button_label_check<GamepadButtonLabel::Square, SDL_GAMEPAD_BUTTON_LABEL_SQUARE>();
      gamepad_button_label_check<GamepadButtonLabel::Triangle, SDL_GAMEPAD_BUTTON_LABEL_TRIANGLE>();

      gamepad_connection_state_check<GamepadConnectionState::Invalid, SDL_JOYSTICK_CONNECTION_INVALID>();
      gamepad_connection_state_check<GamepadConnectionState::Unknown, SDL_JOYSTICK_CONNECTION_UNKNOWN>();
      gamepad_connection_state_check<GamepadConnectionState::Wired, SDL_JOYSTICK_CONNECTION_WIRED>();
      gamepad_connection_state_check<GamepadConnectionState::Wireless, SDL_JOYSTICK_CONNECTION_WIRELESS>();
    }

  } // namespace

  /*
   * GamepadDevice
   */

  bool GamepadDevice::connected()
  {
    assert(m_gamepad != nullptr);
    return SDL_GamepadConnected(m_gamepad);
  }

  GamepadId GamepadDevice::id()
  {
    assert(m_gamepad != nullptr);
    return static_cast<GamepadId>(SDL_GetGamepadID(m_gamepad));
  }

  GamepadType GamepadDevice::type()
  {
    assert(m_gamepad != nullptr);
    return static_cast<GamepadType>(SDL_GetGamepadType(m_gamepad));
  }

  GamepadType GamepadDevice::real_type()
  {
    assert(m_gamepad != nullptr);
    return static_cast<GamepadType>(SDL_GetRealGamepadType(m_gamepad));
  }

  bool GamepadDevice::has_axis(GamepadAxis axis)
  {
    assert(m_gamepad != nullptr);
    return SDL_GamepadHasAxis(m_gamepad, static_cast<SDL_GamepadAxis>(axis));
  }

  bool GamepadDevice::has_button(GamepadButton button)
  {
    assert(m_gamepad != nullptr);
    return SDL_GamepadHasButton(m_gamepad, static_cast<SDL_GamepadButton>(button));
  }

  GamepadButtonLabel GamepadDevice::button_label(GamepadButton button)
  {
    assert(m_gamepad != nullptr);
    return static_cast<GamepadButtonLabel>(SDL_GetGamepadButtonLabel(m_gamepad, static_cast<SDL_GamepadButton>(button)));
  }

  GamepadConnectionState GamepadDevice::connection_state()
  {
    assert(m_gamepad != nullptr);
    return static_cast<GamepadConnectionState>(SDL_GetGamepadConnectionState(m_gamepad));
  }

  uint16_t GamepadDevice::vendor()
  {
    assert(m_gamepad != nullptr);
    return SDL_GetGamepadVendor(m_gamepad);
  }

  uint16_t GamepadDevice::product()
  {
    assert(m_gamepad != nullptr);
    return SDL_GetGamepadProduct(m_gamepad);
  }

  uint16_t GamepadDevice::product_version()
  {
    assert(m_gamepad != nullptr);
    return SDL_GetGamepadProductVersion(m_gamepad);
  }

  uint16_t GamepadDevice::firmware_version()
  {
    assert(m_gamepad != nullptr);
    return SDL_GetGamepadFirmwareVersion(m_gamepad);
  }

  ZString GamepadDevice::name()
  {
    assert(m_gamepad != nullptr);
    return SDL_GetGamepadName(m_gamepad);
  }

  ZString GamepadDevice::path()
  {
    assert(m_gamepad != nullptr);
    return SDL_GetGamepadPath(m_gamepad);
  }

  ZString GamepadDevice::serial()
  {
    assert(m_gamepad != nullptr);
    return SDL_GetGamepadSerial(m_gamepad);
  }

  Id GamepadDevice::serial_id()
  {
    assert(m_gamepad != nullptr);
    return gf::hash_string(serial());
  }

  void GamepadDevice::close()
  {
    assert(m_gamepad != nullptr);
    SDL_CloseGamepad(m_gamepad);
    m_gamepad = nullptr;
  }

  GamepadDevice::GamepadDevice(SDL_Gamepad* gamepad)
  : m_gamepad(gamepad)
  {
  }

  /*
   * Gamepad
   */

  GamepadDevice Gamepad::open(GamepadId id)
  {
    Log::debug("New gamepad with id {}", uint32_t(id));
    Log::debug("\tname: '{}'", name(id).c_str());
    Log::debug("\tpath: '{}'", path(id).c_str());
    Log::debug("\tvendor: {:04X}", vendor(id));
    Log::debug("\tproduct: {:04X}", product(id));
    Log::debug("\tversion: {:04X}", product_version(id));

    SDL_Gamepad* gamepad = SDL_OpenGamepad(static_cast<SDL_JoystickID>(id));

    if (gamepad == nullptr) {
      Log::error("Could not open gamepad with id {}: {}", uint32_t(id), SDL_GetError());
    } else {
      Log::debug("\tserial: {:016X}", uint64_t(GamepadDevice(gamepad).serial_id()));
    }

    return { gamepad };
  }

  GamepadDevice Gamepad::from_id(GamepadId id)
  {
    SDL_Gamepad* gamepad = SDL_GetGamepadFromID(static_cast<SDL_JoystickID>(id));

    if (gamepad == nullptr) {
      Log::error("Could not get gamepad from id {}: {}", uint32_t(id), SDL_GetError());
    }

    return { gamepad };
  }

  ZString Gamepad::name(GamepadId id)
  {
    return SDL_GetGamepadNameForID(static_cast<SDL_JoystickID>(id));
  }

  ZString Gamepad::path(GamepadId id)
  {
    return SDL_GetGamepadPathForID(static_cast<SDL_JoystickID>(id));
  }

  GamepadType Gamepad::type(GamepadId id)
  {
    return static_cast<GamepadType>(SDL_GetGamepadTypeForID(static_cast<SDL_JoystickID>(id)));
  }

  GamepadType Gamepad::real_type(GamepadId id)
  {
    return static_cast<GamepadType>(SDL_GetRealGamepadTypeForID(static_cast<SDL_JoystickID>(id)));
  }

  uint16_t Gamepad::vendor(GamepadId id)
  {
    return SDL_GetGamepadVendorForID(static_cast<SDL_JoystickID>(id));
  }

  uint16_t Gamepad::product(GamepadId id)
  {
    return SDL_GetGamepadProductForID(static_cast<SDL_JoystickID>(id));
  }

  uint16_t Gamepad::product_version(GamepadId id)
  {
    return SDL_GetGamepadProductVersionForID(static_cast<SDL_JoystickID>(id));
  }

  ZString Gamepad::axis_name(GamepadAxis axis)
  {
    return SDL_GetGamepadStringForAxis(static_cast<SDL_GamepadAxis>(axis));
  }

  ZString Gamepad::button_name(GamepadButton button)
  {
    return SDL_GetGamepadStringForButton(static_cast<SDL_GamepadButton>(button));
  }

  ZString Gamepad::type_name(GamepadType type)
  {
    return SDL_GetGamepadStringForType(static_cast<SDL_GamepadType>(type));
  }

  GamepadButtonLabel Gamepad::button_label_for_type(GamepadType type, GamepadButton button)
  {
    return static_cast<GamepadButtonLabel>(SDL_GetGamepadButtonLabelForType(static_cast<SDL_GamepadType>(type), static_cast<SDL_GamepadButton>(button)));
  }

} // namespace gf
