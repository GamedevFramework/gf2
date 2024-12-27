// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_EVENT_H
#define GF_EVENT_H

#include <cstdint>

#include <array>
#include <optional>
#include <variant>

#include <gf2/core/GamepadTypes.h>
#include <gf2/core/Keycode.h>
#include <gf2/core/Modifier.h>
#include <gf2/core/MouseTypes.h>
#include <gf2/core/Scancode.h>
#include <gf2/core/TouchTypes.h>
#include <gf2/core/Vec2.h>

#include "GraphicsApi.h"

namespace gf {

  enum class EventType : uint8_t {
    None,

    Quit,

    WindowShown,
    WindowHidden,
    WindowExposed,
    WindowMoved,
    WindowResized,
    // WindowPixelSizeChanged, // TODO: SDL3
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
    TextEntered,
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

  struct GF_GRAPHICS_API QuitEvent {
  };

  struct GF_GRAPHICS_API WindowShownEvent {
    uint32_t window_id;
  };

  struct GF_GRAPHICS_API WindowHiddenEvent {
    uint32_t window_id;
  };

  struct GF_GRAPHICS_API WindowExposedEvent {
    uint32_t window_id;
  };

  struct GF_GRAPHICS_API WindowMovedEvent {
    uint32_t window_id;
    Vec2I position;
  };

  struct GF_GRAPHICS_API WindowResizedEvent {
    uint32_t window_id;
    Vec2I size;
  };

  // struct GF_GRAPHICS_API WindowPixelSizeChangedEvent {
  //   uint32_t window_id;
  //   Vec2I size;
  // };

  struct GF_GRAPHICS_API WindowMinimizedEvent {
    uint32_t window_id;
  };

  struct GF_GRAPHICS_API WindowMaximizedEvent {
    uint32_t window_id;
  };

  struct GF_GRAPHICS_API WindowRestoredEvent {
    uint32_t window_id;
  };

  struct GF_GRAPHICS_API MouseFocusGainedEvent {
    uint32_t window_id;
  };

  struct GF_GRAPHICS_API MouseFocusLostEvent {
    uint32_t window_id;
  };

  struct GF_GRAPHICS_API KeyboardFocusGainedEvent {
    uint32_t window_id;
  };

  struct GF_GRAPHICS_API KeyboardFocusLostEvent {
    uint32_t window_id;
  };

  struct GF_GRAPHICS_API WindowCloseRequestedEvent {
    uint32_t window_id;
  };

  struct GF_GRAPHICS_API KeyPressedEvent {
    uint32_t window_id;
    Keycode keycode;
    Scancode scancode;
    Flags<Modifier> modifiers;
  };

  struct GF_GRAPHICS_API KeyRepeatedEvent {
    uint32_t window_id;
    Keycode keycode;
    Scancode scancode;
    Flags<Modifier> modifiers;
    uint8_t repeat;
  };

  struct GF_GRAPHICS_API KeyReleasedEvent {
    uint32_t window_id;
    Keycode keycode;
    Scancode scancode;
    Flags<Modifier> modifiers;
  };


  // struct GF_GRAPHICS_API TextEditedEvent {
  //   uint32_t window_id;
  //   std::string_view text;
  //   int32_t start;
  //   int32_t length;
  // };

  struct GF_GRAPHICS_API TextEnteredEvent {
    uint32_t window_id;
    std::array<char, 32> text; // TODO: SDL3: changed to a std::string_view
  };

  // struct GF_GRAPHICS_API TextCandidatesShownEvent {
  //   uint32_t window_id;
  //   std::array<std::string_view, 10> candidates;
  //   int32_t count;
  //   int32_t selected;
  //   bool horizontal;
  // };

  struct GF_GRAPHICS_API MouseMovedEvent {
    uint32_t window_id;
    MouseId mouse_id;
    Vec2I position;
    Vec2I motion;
  };

  struct GF_GRAPHICS_API MouseButtonPressedEvent {
    uint32_t window_id;
    MouseId mouse_id;
    MouseButton button;
    Vec2I position;
    uint8_t clicks;
  };

  struct GF_GRAPHICS_API MouseButtonReleasedEvent {
    uint32_t window_id;
    MouseId mouse_id;
    MouseButton button;
    Vec2I position;
    uint8_t clicks;
  };

  struct GF_GRAPHICS_API MouseWheelScrolledEvent {
    uint32_t window_id;
    MouseId mouse_id;
    Vec2I offset;
    MouseWheelDirection direction;
  };

  struct GF_GRAPHICS_API GamepadAxisMovedEvent {
    GamepadId gamepad_id;
    GamepadAxis axis;
    int16_t value;
  };

  struct GF_GRAPHICS_API GamepadButtonPressedEvent {
    GamepadId gamepad_id;
    GamepadButton button;
  };

  struct GF_GRAPHICS_API GamepadButtonReleasedEvent {
    GamepadId gamepad_id;
    GamepadButton button;
  };

  struct GF_GRAPHICS_API GamepadConnectedEvent {
    GamepadHwId gamepad_hw_id;
  };

  struct GF_GRAPHICS_API GamepadDisconnectedEvent {
    GamepadId gamepad_id;
  };

  struct GF_GRAPHICS_API TouchPressedEvent {
    uint32_t window_id;
    TouchId touch_id;
    FingerId finger;
    Vec2F location;
    Vec2F motion;
    float pressure;
  };

  struct GF_GRAPHICS_API TouchReleasedEvent {
    uint32_t window_id;
    TouchId touch_id;
    FingerId finger;
    Vec2F location;
    Vec2F motion;
    float pressure;
  };

  struct GF_GRAPHICS_API TouchMovedEvent {
    uint32_t window_id;
    TouchId touch_id;
    FingerId finger;
    Vec2F location;
    Vec2F motion;
    float pressure;
  };

  class GF_GRAPHICS_API Event {
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
      return static_cast<EventType>(m_data.index());
    }

    template<typename T>
    constexpr bool is() const
    {
      return std::holds_alternative<T>(m_data);
    }

    template<typename T>
    constexpr T& as()
    {
      return std::get<T>(m_data);
    }

    template<typename T>
    constexpr const T& as() const
    {
      return std::get<T>(m_data);
    }

    template<EventType E>
    constexpr auto& from()
    {
      return std::get<static_cast<std::size_t>(E)>(m_data);
    }

    template<EventType E>
    constexpr const auto& from() const
    {
      return std::get<static_cast<std::size_t>(E)>(m_data);
    }

    static std::optional<Event> poll();
    static Event wait();

  private:
    uint64_t m_timestamp = 0;

    std::variant<
      std::monostate,
      QuitEvent,

      WindowShownEvent,
      WindowHiddenEvent,
      WindowExposedEvent,
      WindowMovedEvent,
      WindowResizedEvent,
      // WindowPixelSizeChangedEvent,
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
      TextEnteredEvent,
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
  };

} // namespace gf

#endif // GF_EVENT_H
