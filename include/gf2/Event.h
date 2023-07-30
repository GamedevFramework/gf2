// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_EVENT_H
#define GF_EVENT_H

#include <cstdint>

#include <array>
#include <optional>

#include "Gamepad.h"
#include "GraphicsApi.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Touch.h"
#include "Vec2.h"

namespace gf {

  enum EventType : uint8_t {
    Quit,

    WindowShown,
    WindowHidden,
    WindowExposed,
    WindowMinimized,
    WindowMaximized,
    WindowRestored,
    WindowClosed,
    WindowResized,

    KeyPressed,
    KeyRepeated,
    KeyReleased,

    KeyboardFocusGained,
    KeyboardFocusLost,

    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseWheelScrolled,
    MouseFocusGained,
    MouseFocusLost,

    GamepadAxisMoved,
    GamepadButtonPressed,
    GamepadButtonReleased,
    GamepadConnected,
    GamepadDisconnected,

    TextEntered,

    TouchBegan,
    TouchMoved,
    TouchEnded,
  };

  struct GF_GRAPHICS_API WindowEvent {
    uint32_t window_id;
  };

  struct GF_GRAPHICS_API ResizeEvent {
    uint32_t window_id;
    Vec2<int32_t> size;
  };

  struct GF_GRAPHICS_API KeyEvent {
    uint32_t window_id;
    Keycode keycode;
    Scancode scancode;
    Flags<Modifier> modifiers;
    uint8_t repeat;
    bool pressed;
  };

  struct GF_GRAPHICS_API TextEvent {
    uint32_t window_id;
    std::array<char, 32> text;
  };

  struct GF_GRAPHICS_API MouseButtonEvent {
    uint32_t window_id;
    MouseId mouse_id;
    MouseButton button;
    Vec2<int32_t> coordinates;
    uint8_t clicks;
    bool pressed;
  };

  struct GF_GRAPHICS_API MouseMotionEvent {
    uint32_t window_id;
    MouseId mouse_id;
    Vec2<int32_t> coordinates;
    Vec2<int32_t> motion;
  };

  struct GF_GRAPHICS_API MouseWheelEvent {
    uint32_t window_id;
    MouseId mouse_id;
    Vec2<int32_t> offset;
  };

  struct GF_GRAPHICS_API GamepadAxisEvent {
    GamepadId gamepad_id;
    GamepadAxis axis;
    int16_t value;
  };

  struct GF_GRAPHICS_API GamepadButtonEvent {
    GamepadId gamepad_id;
    GamepadButton button;
    bool pressed;
  };

  struct GF_GRAPHICS_API GamepadConnectionEvent {
    GamepadHwId gamepad_hw_id;
  };

  struct GF_GRAPHICS_API GamepadDisconnectionEvent {
    GamepadId gamepad_id;
  };

  struct GF_GRAPHICS_API TouchEvent {
    uint32_t window_id;
    TouchId touch_id;
    FingerId finger;
    Vec2F coordinates;
    Vec2F motion;
    float pressure;
  };

  struct GF_GRAPHICS_API Event {
    EventType type;
    uint32_t timestamp;

    union {
      WindowEvent window;
      ResizeEvent resize;
      KeyEvent key;
      TextEvent text;
      MouseButtonEvent mouse_button;
      MouseMotionEvent mouse_motion;
      MouseWheelEvent mouse_wheel;
      GamepadAxisEvent gamepad_axis;
      GamepadButtonEvent gamepad_button;
      GamepadConnectionEvent gamepad_connection;
      GamepadDisconnectionEvent gamepad_disconnection;
      TouchEvent touch;
    };

    static std::optional<Event> poll();
    static Event wait();
  };

} // namespace gf

#endif // GF_EVENT_H
