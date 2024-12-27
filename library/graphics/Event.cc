// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/Event.h>

#include <algorithm>
#include <tuple>
#include <type_traits>

#include <SDL2/SDL.h>

#include <gf2/core/Log.h>

namespace gf {

  namespace {

    std::optional<Event> to_window_event(const SDL_Event& raw_event)
    {
      assert(raw_event.type == SDL_WINDOWEVENT);
      const uint64_t timestamp = raw_event.window.timestamp;
      const uint32_t window_id = raw_event.window.windowID;

      switch (raw_event.window.event) {
        case SDL_WINDOWEVENT_SHOWN:
          {
            WindowShownEvent data = {};
            data.window_id = window_id;
            return Event(timestamp, data);
          }
        case SDL_WINDOWEVENT_HIDDEN:
          {
            WindowHiddenEvent data = {};
            data.window_id = window_id;
            return Event(timestamp, data);
          }
        case SDL_WINDOWEVENT_EXPOSED:
          {
            WindowExposedEvent data = {};
            data.window_id = window_id;
            return Event(timestamp, data);
          }
        case SDL_WINDOWEVENT_MOVED:
          {
            WindowMovedEvent data = {};
            data.window_id = window_id;
            data.position.x = raw_event.window.data1;
            data.position.y = raw_event.window.data2;
            return Event(timestamp, data);
          }
        case SDL_WINDOWEVENT_SIZE_CHANGED:
          {
            WindowResizedEvent data = {};
            data.window_id = window_id;
            data.size.w = raw_event.window.data1;
            data.size.h = raw_event.window.data2;
            return Event(timestamp, data);
          }
        case SDL_WINDOWEVENT_MINIMIZED:
          {
            WindowMinimizedEvent data = {};
            data.window_id = window_id;
            return Event(timestamp, data);
          }
        case SDL_WINDOWEVENT_MAXIMIZED:
          {
            WindowMaximizedEvent data = {};
            data.window_id = window_id;
            return Event(timestamp, data);
          }
        case SDL_WINDOWEVENT_RESTORED:
          {
            WindowRestoredEvent data = {};
            data.window_id = window_id;
            return Event(timestamp, data);
          }
        case SDL_WINDOWEVENT_ENTER:
          {
            MouseFocusGainedEvent data = {};
            data.window_id = window_id;
            return Event(timestamp, data);
          }
        case SDL_WINDOWEVENT_LEAVE:
          {
            MouseFocusLostEvent data = {};
            data.window_id = window_id;
            return Event(timestamp, data);
          }
        case SDL_WINDOWEVENT_FOCUS_GAINED:
          {
            KeyboardFocusGainedEvent data = {};
            data.window_id = window_id;
            return Event(timestamp, data);
          }
        case SDL_WINDOWEVENT_FOCUS_LOST:
          {
            KeyboardFocusLostEvent data = {};
            data.window_id = window_id;
            return Event(timestamp, data);
          }
        case SDL_WINDOWEVENT_CLOSE:
          {
            WindowCloseRequestedEvent data = {};
            data.window_id = window_id;
            return Event(timestamp, data);
          }
        default:
          break;
      }

      return std::nullopt;
    }

    std::optional<Event> to_event(const SDL_Event& raw_event)
    {
      const uint64_t timestamp = raw_event.common.timestamp;

      switch (raw_event.type) {
        case SDL_WINDOWEVENT:
          return to_window_event(raw_event);

        case SDL_QUIT:
          return Event(timestamp, QuitEvent());
          break;

        case SDL_KEYDOWN:
          assert(raw_event.key.state == SDL_PRESSED);

          if (raw_event.key.repeat > 0) {
            KeyRepeatedEvent data = {};
            data.window_id = raw_event.key.windowID;
            data.keycode = static_cast<Keycode>(raw_event.key.keysym.sym);
            data.scancode = static_cast<Scancode>(raw_event.key.keysym.scancode);
            data.modifiers = static_cast<Modifier>(raw_event.key.keysym.mod);
            data.repeat = raw_event.key.repeat;
            return Event(timestamp, data);
          }

          {
            KeyPressedEvent data = {};
            data.window_id = raw_event.key.windowID;
            data.keycode = static_cast<Keycode>(raw_event.key.keysym.sym);
            data.scancode = static_cast<Scancode>(raw_event.key.keysym.scancode);
            data.modifiers = static_cast<Modifier>(raw_event.key.keysym.mod);
            return Event(timestamp, data);
          }

        case SDL_KEYUP:
          assert(raw_event.key.state == SDL_RELEASED);
          {
            KeyReleasedEvent data = {};
            data.window_id = raw_event.key.windowID;
            data.keycode = static_cast<Keycode>(raw_event.key.keysym.sym);
            data.scancode = static_cast<Scancode>(raw_event.key.keysym.scancode);
            data.modifiers = static_cast<Modifier>(raw_event.key.keysym.mod);
            return Event(timestamp, data);
          }

        case SDL_TEXTINPUT:
          {
            TextEnteredEvent data = {};
            data.window_id = raw_event.text.windowID;
            static_assert(std::tuple_size_v<decltype(data.text)> == std::extent_v<decltype(raw_event.text.text)>, "Buffer size mismatch.");
            std::copy_n(std::begin(raw_event.text.text), data.text.size(), data.text.data());
            return Event(timestamp, data);
          }

        case SDL_MOUSEMOTION:
          {
            MouseMovedEvent data = {};
            data.window_id = raw_event.motion.windowID;
            data.mouse_id = static_cast<MouseId>(raw_event.motion.which);
            data.position.x = raw_event.motion.x;
            data.position.y = raw_event.motion.y;
            data.motion.x = raw_event.motion.xrel;
            data.motion.y = raw_event.motion.yrel;
            return Event(timestamp, data);
          }

        case SDL_MOUSEBUTTONDOWN:
          assert(raw_event.button.state == SDL_PRESSED);
          {
            MouseButtonPressedEvent data = {};
            data.window_id = raw_event.button.windowID;
            data.mouse_id = static_cast<MouseId>(raw_event.button.which);
            data.button = static_cast<MouseButton>(raw_event.button.button);
            data.position.x = raw_event.button.x;
            data.position.y = raw_event.button.y;
            data.clicks = raw_event.button.clicks;
            return Event(timestamp, data);
          }

        case SDL_MOUSEBUTTONUP:
          assert(raw_event.button.state == SDL_RELEASED);
          {
            MouseButtonReleasedEvent data = {};
            data.window_id = raw_event.button.windowID;
            data.mouse_id = static_cast<MouseId>(raw_event.button.which);
            data.button = static_cast<MouseButton>(raw_event.button.button);
            data.position.x = raw_event.button.x;
            data.position.y = raw_event.button.y;
            data.clicks = raw_event.button.clicks;
            return Event(timestamp, data);
          }

        case SDL_MOUSEWHEEL:
          {
            MouseWheelScrolledEvent data = {};
            data.window_id = raw_event.wheel.windowID;
            data.mouse_id = static_cast<MouseId>(raw_event.wheel.which);
            data.offset.x = raw_event.wheel.x;
            data.offset.y = raw_event.wheel.y;
            data.direction = static_cast<MouseWheelDirection>(raw_event.wheel.direction);
            return Event(timestamp, data);
          }

        case SDL_CONTROLLERAXISMOTION:
          {
            GamepadAxisMovedEvent data = {};
            data.gamepad_id = static_cast<GamepadId>(raw_event.caxis.which);
            data.axis = static_cast<GamepadAxis>(raw_event.caxis.axis);
            data.value = raw_event.caxis.value;
            return Event(timestamp, data);
          }

        case SDL_CONTROLLERBUTTONDOWN:
          {
            assert(raw_event.cbutton.state == SDL_PRESSED);
            GamepadButtonPressedEvent data = {};
            data.gamepad_id = static_cast<GamepadId>(raw_event.cbutton.which);
            data.button = static_cast<GamepadButton>(raw_event.cbutton.button);
            return Event(timestamp, data);
          }

        case SDL_CONTROLLERBUTTONUP:
          {
            assert(raw_event.cbutton.state == SDL_RELEASED);
            GamepadButtonReleasedEvent data = {};
            data.gamepad_id = static_cast<GamepadId>(raw_event.cbutton.which);
            data.button = static_cast<GamepadButton>(raw_event.cbutton.button);
            return Event(timestamp, data);
          }

        case SDL_CONTROLLERDEVICEADDED:
          {
            assert(SDL_IsGameController(raw_event.cdevice.which));
            GamepadConnectedEvent data = {};
            data.gamepad_hw_id = static_cast<GamepadHwId>(raw_event.cdevice.which);
            return Event(timestamp, data);
          }

        case SDL_CONTROLLERDEVICEREMOVED:
          {
            GamepadDisconnectedEvent data = {};
            data.gamepad_id = static_cast<GamepadId>(raw_event.cdevice.which);
            return Event(timestamp, data);
          }

        case SDL_FINGERDOWN:
          {
            TouchPressedEvent data = {};
            data.window_id = raw_event.tfinger.windowID;
            data.touch_id = static_cast<TouchId>(raw_event.tfinger.touchId);
            data.finger = static_cast<FingerId>(raw_event.tfinger.fingerId);
            data.location.x = raw_event.tfinger.x;
            data.location.y = raw_event.tfinger.y;
            data.motion.x = raw_event.tfinger.dx;
            data.motion.y = raw_event.tfinger.dy;
            data.pressure = raw_event.tfinger.pressure;
            return Event(timestamp, data);
          }

        case SDL_FINGERUP:
          {
            TouchReleasedEvent data = {};
            data.window_id = raw_event.tfinger.windowID;
            data.touch_id = static_cast<TouchId>(raw_event.tfinger.touchId);
            data.finger = static_cast<FingerId>(raw_event.tfinger.fingerId);
            data.location.x = raw_event.tfinger.x;
            data.location.y = raw_event.tfinger.y;
            data.motion.x = raw_event.tfinger.dx;
            data.motion.y = raw_event.tfinger.dy;
            data.pressure = raw_event.tfinger.pressure;
            return Event(timestamp, data);
          }

        case SDL_FINGERMOTION:
          {
            TouchMovedEvent data = {};
            data.window_id = raw_event.tfinger.windowID;
            data.touch_id = static_cast<TouchId>(raw_event.tfinger.touchId);
            data.finger = static_cast<FingerId>(raw_event.tfinger.fingerId);
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

  std::optional<Event> Event::poll()
  {
    for (;;) {
      SDL_Event raw_event = {};
      const int status = SDL_PollEvent(&raw_event);

      if (status == 0) {
        break;
      }

      auto maybe_event = to_event(raw_event);

      if (maybe_event) {
        return maybe_event;
      }
    }

    return std::nullopt;
  }

  Event Event::wait()
  {
    for (;;) {
      SDL_Event raw_event = {};
      const int status = SDL_WaitEvent(&raw_event);

      if (status == 0) {
        Log::fatal("Error when waiting an event: '{}'", SDL_GetError());
      }

      auto maybe_event = to_event(raw_event);

      if (maybe_event) {
        return *maybe_event;
      }
    }
  }

} // namespace gf
