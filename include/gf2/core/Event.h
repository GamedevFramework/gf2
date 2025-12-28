// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_EVENT_H
#define GF_EVENT_H

#include <cstdint>

#include <string_view>
#include <variant>

#include "CoreApi.h"
#include "GamepadTypes.h"
#include "Keycode.h"
#include "Modifier.h"
#include "MouseTypes.h"
#include "Scancode.h"
#include "TaggedVariant.h"
#include "TouchTypes.h"
#include "Vec2.h"
#include "WindowId.h"

namespace gf {

  enum class EventType : uint8_t {
    None,

    Quit,

    WindowShown,
    WindowHidden,
    WindowExposed,
    WindowMoved,
    WindowResized,
    WindowPixelSizeChanged,
    WindowMinimized,
    WindowMaximized,
    WindowRestored,
    MouseFocusGained,
    MouseFocusLost,
    KeyboardFocusGained,
    KeyboardFocusLost,
    WindowCloseRequested,

    KeyPressed,
    KeyRepeated,
    KeyReleased,

    // TextEdited, // TODO: SDL3
    TextInput,
    // TextCandidatesShown, // TODO: SDL3

    MouseMoved,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseWheelScrolled,

    GamepadAxisMoved,
    GamepadButtonPressed,
    GamepadButtonReleased,
    GamepadConnected,
    GamepadDisconnected,

    TouchPressed,
    TouchReleased,
    TouchMoved,
  };

  // clang-format off
  struct GF_CORE_API QuitEvent {
  };
  // clang-format on

  struct GF_CORE_API WindowShownEvent {
    WindowId window_id;
  };

  struct GF_CORE_API WindowHiddenEvent {
    WindowId window_id;
  };

  struct GF_CORE_API WindowExposedEvent {
    WindowId window_id;
  };

  struct GF_CORE_API WindowMovedEvent {
    WindowId window_id;
    Vec2I position;
  };

  struct GF_CORE_API WindowResizedEvent {
    WindowId window_id;
    Vec2I size;
  };

  struct GF_CORE_API WindowPixelSizeChangedEvent {
    WindowId window_id;
    Vec2I size;
  };

  struct GF_CORE_API WindowMinimizedEvent {
    WindowId window_id;
  };

  struct GF_CORE_API WindowMaximizedEvent {
    WindowId window_id;
  };

  struct GF_CORE_API WindowRestoredEvent {
    WindowId window_id;
  };

  struct GF_CORE_API MouseFocusGainedEvent {
    WindowId window_id;
  };

  struct GF_CORE_API MouseFocusLostEvent {
    WindowId window_id;
  };

  struct GF_CORE_API KeyboardFocusGainedEvent {
    WindowId window_id;
  };

  struct GF_CORE_API KeyboardFocusLostEvent {
    WindowId window_id;
  };

  struct GF_CORE_API WindowCloseRequestedEvent {
    WindowId window_id;
  };

  struct GF_CORE_API KeyPressedEvent {
    WindowId window_id;
    Keycode keycode;
    Scancode scancode;
    Flags<Modifier> modifiers;
  };

  struct GF_CORE_API KeyRepeatedEvent {
    WindowId window_id;
    Keycode keycode;
    Scancode scancode;
    Flags<Modifier> modifiers;
    bool repeat;
  };

  struct GF_CORE_API KeyReleasedEvent {
    WindowId window_id;
    Keycode keycode;
    Scancode scancode;
    Flags<Modifier> modifiers;
  };

  // struct GF_CORE_API TextEditedEvent {
  //   WindowId window_id;
  //   std::string_view text;
  //   int32_t start;
  //   int32_t length;
  // };

  struct GF_CORE_API TextInputEvent {
    WindowId window_id;
    std::string_view text;
  };

  // struct GF_CORE_API TextCandidatesShownEvent {
  //   WindowId window_id;
  //   std::array<std::string_view, 10> candidates;
  //   int32_t count;
  //   int32_t selected;
  //   bool horizontal;
  // };

  struct GF_CORE_API MouseMovedEvent {
    WindowId window_id;
    MouseId mouse_id;
    Vec2F position;
    Vec2F motion;
  };

  struct GF_CORE_API MouseButtonPressedEvent {
    WindowId window_id;
    MouseId mouse_id;
    MouseButton button;
    uint8_t clicks;
    Vec2F position;
  };

  struct GF_CORE_API MouseButtonReleasedEvent {
    WindowId window_id;
    MouseId mouse_id;
    MouseButton button;
    uint8_t clicks;
    Vec2F position;
  };

  struct GF_CORE_API MouseWheelScrolledEvent {
    WindowId window_id;
    MouseId mouse_id;
    Vec2F offset;
    MouseWheelDirection direction;
    Vec2F position;
  };

  struct GF_CORE_API GamepadAxisMovedEvent {
    GamepadId gamepad_id;
    GamepadAxis axis;
    int16_t value;
  };

  struct GF_CORE_API GamepadButtonPressedEvent {
    GamepadId gamepad_id;
    GamepadButton button;
  };

  struct GF_CORE_API GamepadButtonReleasedEvent {
    GamepadId gamepad_id;
    GamepadButton button;
  };

  struct GF_CORE_API GamepadConnectedEvent {
    GamepadId gamepad_id;
  };

  struct GF_CORE_API GamepadDisconnectedEvent {
    GamepadId gamepad_id;
  };

  struct GF_CORE_API TouchPressedEvent {
    WindowId window_id;
    TouchId touch_id;
    FingerId finger;
    Vec2F location;
    Vec2F motion;
    float pressure;
  };

  struct GF_CORE_API TouchReleasedEvent {
    WindowId window_id;
    TouchId touch_id;
    FingerId finger;
    Vec2F location;
    Vec2F motion;
    float pressure;
  };

  struct GF_CORE_API TouchMovedEvent {
    WindowId window_id;
    TouchId touch_id;
    FingerId finger;
    Vec2F location;
    Vec2F motion;
    float pressure;
  };

  class GF_CORE_API Event {
  public:
    constexpr Event() = default;

    template<typename T>
    constexpr Event(uint64_t timestamp, T&& data)
    : m_timestamp(timestamp)
    , m_data(std::forward<T>(data))
    {
    }

    constexpr uint64_t timestamp() const
    {
      return m_timestamp;
    }

    constexpr EventType type() const
    {
      return m_data.type();
    }

    template<typename T>
    constexpr bool is() const
    {
      return m_data.is<T>();
    }

    template<typename T>
    constexpr T& as()
    {
      return std::get<T>(m_data.raw());
    }

    template<typename T>
    constexpr const T& as() const
    {
      return std::get<T>(m_data.raw());
    }

    template<EventType E>
    constexpr auto& from()
    {
      return m_data.from<E>();
    }

    template<EventType E>
    constexpr const auto& from() const
    {
      return m_data.from<E>();
    }

  private:
    uint64_t m_timestamp = 0;

    // clang-format off
    TaggedVariant<EventType,
      QuitEvent,

      WindowShownEvent,
      WindowHiddenEvent,
      WindowExposedEvent,
      WindowMovedEvent,
      WindowResizedEvent,
      WindowPixelSizeChangedEvent,
      WindowMinimizedEvent,
      WindowMaximizedEvent,
      WindowRestoredEvent,
      MouseFocusGainedEvent,
      MouseFocusLostEvent,
      KeyboardFocusGainedEvent,
      KeyboardFocusLostEvent,
      WindowCloseRequestedEvent,

      KeyPressedEvent,
      KeyRepeatedEvent,
      KeyReleasedEvent,

      // TextEditedEvent,
      TextInputEvent,
      // TextCandidatesShownEvent,

      MouseMovedEvent,
      MouseButtonPressedEvent,
      MouseButtonReleasedEvent,
      MouseWheelScrolledEvent,

      GamepadAxisMovedEvent,
      GamepadButtonPressedEvent,
      GamepadButtonReleasedEvent,
      GamepadConnectedEvent,
      GamepadDisconnectedEvent,

      TouchPressedEvent,
      TouchReleasedEvent,
      TouchMovedEvent
    > m_data;
    // clang-format on
  };

} // namespace gf

#endif // GF_EVENT_H
