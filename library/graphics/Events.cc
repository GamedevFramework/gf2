// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Events.h>

#include <algorithm>
#include <tuple>
#include <type_traits>

#include <SDL3/SDL.h>

#include <gf2/core/Log.h>

namespace gf {

  namespace {

    std::optional<Event> to_event(const SDL_Event& raw_event)
    {
      const uint64_t timestamp = raw_event.common.timestamp;

      switch (raw_event.type) {
        case SDL_EVENT_QUIT:
          return Event(timestamp, QuitEvent());
          break;

        case SDL_EVENT_WINDOW_SHOWN:
          {
            WindowShownEvent data = {};
            data.window_id = WindowId{raw_event.window.windowID};
            return Event(timestamp, data);
          }
        case SDL_EVENT_WINDOW_HIDDEN:
          {
            WindowHiddenEvent data = {};
            data.window_id = WindowId{raw_event.window.windowID};
            return Event(timestamp, data);
          }
        case SDL_EVENT_WINDOW_EXPOSED:
          {
            WindowExposedEvent data = {};
            data.window_id = WindowId{raw_event.window.windowID};
            return Event(timestamp, data);
          }
        case SDL_EVENT_WINDOW_MOVED:
          {
            WindowMovedEvent data = {};
            data.window_id = WindowId{raw_event.window.windowID};
            data.position.x = raw_event.window.data1;
            data.position.y = raw_event.window.data2;
            return Event(timestamp, data);
          }
        case SDL_EVENT_WINDOW_RESIZED:
          {
            WindowResizedEvent data = {};
            data.window_id = WindowId{raw_event.window.windowID};
            data.size.w = raw_event.window.data1;
            data.size.h = raw_event.window.data2;
            return Event(timestamp, data);
          }
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
          {
            WindowPixelSizeChangedEvent data = {};
            data.window_id = WindowId{raw_event.window.windowID};
            data.size.w = raw_event.window.data1;
            data.size.h = raw_event.window.data2;
            return Event(timestamp, data);
          }
        case SDL_EVENT_WINDOW_MINIMIZED:
          {
            WindowMinimizedEvent data = {};
            data.window_id = WindowId{raw_event.window.windowID};
            return Event(timestamp, data);
          }
        case SDL_EVENT_WINDOW_MAXIMIZED:
          {
            WindowMaximizedEvent data = {};
            data.window_id = WindowId{raw_event.window.windowID};
            return Event(timestamp, data);
          }
        case SDL_EVENT_WINDOW_RESTORED:
          {
            WindowRestoredEvent data = {};
            data.window_id = WindowId{raw_event.window.windowID};
            return Event(timestamp, data);
          }
        case SDL_EVENT_WINDOW_MOUSE_ENTER:
          {
            MouseFocusGainedEvent data = {};
            data.window_id = WindowId{raw_event.window.windowID};
            return Event(timestamp, data);
          }
        case SDL_EVENT_WINDOW_MOUSE_LEAVE:
          {
            MouseFocusLostEvent data = {};
            data.window_id = WindowId{raw_event.window.windowID};
            return Event(timestamp, data);
          }
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
          {
            KeyboardFocusGainedEvent data = {};
            data.window_id = WindowId{raw_event.window.windowID};
            return Event(timestamp, data);
          }
        case SDL_EVENT_WINDOW_FOCUS_LOST:
          {
            KeyboardFocusLostEvent data = {};
            data.window_id = WindowId{raw_event.window.windowID};
            return Event(timestamp, data);
          }
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
          {
            WindowCloseRequestedEvent data = {};
            data.window_id = WindowId{raw_event.window.windowID};
            return Event(timestamp, data);
          }

        case SDL_EVENT_KEY_DOWN:
          assert(raw_event.key.down);

          if (raw_event.key.repeat) {
            KeyRepeatedEvent data = {};
            data.window_id = WindowId{raw_event.key.windowID};
            data.keycode = static_cast<Keycode>(raw_event.key.key);
            data.scancode = static_cast<Scancode>(raw_event.key.scancode);
            data.modifiers = static_cast<Modifier>(raw_event.key.mod);
            data.repeat = raw_event.key.repeat;
            return Event(timestamp, data);
          }

          {
            KeyPressedEvent data = {};
            data.window_id = WindowId{raw_event.key.windowID};
            data.keycode = static_cast<Keycode>(raw_event.key.key);
            data.scancode = static_cast<Scancode>(raw_event.key.scancode);
            data.modifiers = static_cast<Modifier>(raw_event.key.mod);
            return Event(timestamp, data);
          }

        case SDL_EVENT_KEY_UP:
          assert(!raw_event.key.down);
          {
            KeyReleasedEvent data = {};
            data.window_id = WindowId{raw_event.key.windowID};
            data.keycode = static_cast<Keycode>(raw_event.key.key);
            data.scancode = static_cast<Scancode>(raw_event.key.scancode);
            data.modifiers = static_cast<Modifier>(raw_event.key.mod);
            return Event(timestamp, data);
          }

        case SDL_EVENT_TEXT_INPUT:
          {
            TextInputEvent data = {};
            data.window_id = WindowId{raw_event.text.windowID};
            data.text = raw_event.text.text;
            return Event(timestamp, data);
          }

        case SDL_EVENT_MOUSE_MOTION:
          {
            MouseMovedEvent data = {};
            data.window_id = WindowId{raw_event.motion.windowID};
            data.mouse_id = static_cast<MouseId>(raw_event.motion.which);
            data.position.x = raw_event.motion.x;
            data.position.y = raw_event.motion.y;
            data.motion.x = raw_event.motion.xrel;
            data.motion.y = raw_event.motion.yrel;
            return Event(timestamp, data);
          }

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
          assert(raw_event.button.down);
          {
            MouseButtonPressedEvent data = {};
            data.window_id = WindowId{raw_event.button.windowID};
            data.mouse_id = static_cast<MouseId>(raw_event.button.which);
            data.button = static_cast<MouseButton>(raw_event.button.button);
            data.position.x = raw_event.button.x;
            data.position.y = raw_event.button.y;
            data.clicks = raw_event.button.clicks;
            return Event(timestamp, data);
          }

        case SDL_EVENT_MOUSE_BUTTON_UP:
          assert(!raw_event.button.down);
          {
            MouseButtonReleasedEvent data = {};
            data.window_id = WindowId{raw_event.button.windowID};
            data.mouse_id = static_cast<MouseId>(raw_event.button.which);
            data.button = static_cast<MouseButton>(raw_event.button.button);
            data.position.x = raw_event.button.x;
            data.position.y = raw_event.button.y;
            data.clicks = raw_event.button.clicks;
            return Event(timestamp, data);
          }

        case SDL_EVENT_MOUSE_WHEEL:
          {
            MouseWheelScrolledEvent data = {};
            data.window_id = WindowId{raw_event.wheel.windowID};
            data.mouse_id = static_cast<MouseId>(raw_event.wheel.which);
            data.offset.x = raw_event.wheel.x;
            data.offset.y = raw_event.wheel.y;
            data.direction = static_cast<MouseWheelDirection>(raw_event.wheel.direction);
            return Event(timestamp, data);
          }

        case SDL_EVENT_GAMEPAD_AXIS_MOTION:
          {
            GamepadAxisMovedEvent data = {};
            data.gamepad_id = static_cast<GamepadId>(raw_event.gaxis.which);
            data.axis = static_cast<GamepadAxis>(raw_event.gaxis.axis);
            data.value = raw_event.gaxis.value;
            return Event(timestamp, data);
          }

        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
          assert(raw_event.gbutton.down);
          {
            GamepadButtonPressedEvent data = {};
            data.gamepad_id = static_cast<GamepadId>(raw_event.gbutton.which);
            data.button = static_cast<GamepadButton>(raw_event.gbutton.button);
            return Event(timestamp, data);
          }

        case SDL_EVENT_GAMEPAD_BUTTON_UP:
          assert(!raw_event.gbutton.down);
          {
            GamepadButtonReleasedEvent data = {};
            data.gamepad_id = static_cast<GamepadId>(raw_event.gbutton.which);
            data.button = static_cast<GamepadButton>(raw_event.gbutton.button);
            return Event(timestamp, data);
          }

        case SDL_EVENT_GAMEPAD_ADDED:
          {
            GamepadConnectedEvent data = {};
            data.gamepad_id = static_cast<GamepadId>(raw_event.gdevice.which);
            return Event(timestamp, data);
          }

        case SDL_EVENT_GAMEPAD_REMOVED:
          {
            GamepadDisconnectedEvent data = {};
            data.gamepad_id = static_cast<GamepadId>(raw_event.gdevice.which);
            return Event(timestamp, data);
          }

        case SDL_EVENT_FINGER_DOWN:
          {
            TouchPressedEvent data = {};
            data.window_id = WindowId{raw_event.tfinger.windowID};
            data.touch_id = static_cast<TouchId>(raw_event.tfinger.touchID);
            data.finger = static_cast<FingerId>(raw_event.tfinger.fingerID);
            data.location.x = raw_event.tfinger.x;
            data.location.y = raw_event.tfinger.y;
            data.motion.x = raw_event.tfinger.dx;
            data.motion.y = raw_event.tfinger.dy;
            data.pressure = raw_event.tfinger.pressure;
            return Event(timestamp, data);
          }

        case SDL_EVENT_FINGER_UP:
          {
            TouchReleasedEvent data = {};
            data.window_id = WindowId{raw_event.tfinger.windowID};
            data.touch_id = static_cast<TouchId>(raw_event.tfinger.touchID);
            data.finger = static_cast<FingerId>(raw_event.tfinger.fingerID);
            data.location.x = raw_event.tfinger.x;
            data.location.y = raw_event.tfinger.y;
            data.motion.x = raw_event.tfinger.dx;
            data.motion.y = raw_event.tfinger.dy;
            data.pressure = raw_event.tfinger.pressure;
            return Event(timestamp, data);
          }

        case SDL_EVENT_FINGER_MOTION:
          {
            TouchMovedEvent data = {};
            data.window_id = WindowId{raw_event.tfinger.windowID};
            data.touch_id = static_cast<TouchId>(raw_event.tfinger.touchID);
            data.finger = static_cast<FingerId>(raw_event.tfinger.fingerID);
            data.location.x = raw_event.tfinger.x;
            data.location.y = raw_event.tfinger.y;
            data.motion.x = raw_event.tfinger.dx;
            data.motion.y = raw_event.tfinger.dy;
            data.pressure = raw_event.tfinger.pressure;
            return Event(timestamp, data);
          }

        default:
          break;
      };

      return std::nullopt;
    }

  } // namespace

  std::optional<Event> Events::poll()
  {
    for (;;) {
      SDL_Event raw_event = {};

      if (!SDL_PollEvent(&raw_event)) {
        break;
      }

      auto maybe_event = to_event(raw_event);

      if (maybe_event) {
        return maybe_event;
      }
    }

    return std::nullopt;
  }

  Event Events::wait()
  {
    for (;;) {
      SDL_Event raw_event = {};

      if (!SDL_WaitEvent(&raw_event)) {
        Log::fatal("Error when waiting an event: '{}'", SDL_GetError());
      }

      auto maybe_event = to_event(raw_event);

      if (maybe_event) {
        return *maybe_event;
      }
    }
  }

} // namespace gf
