// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Event.h>
// clang-format on

#include <algorithm>
#include <tuple>

#include <SDL2/SDL.h>
#include <gf2/Log.h>

namespace gf {

  namespace {

    std::optional<Event> to_window_event(const SDL_Event& raw_event)
    {
      assert(raw_event.type == SDL_WINDOWEVENT);
      Event event = {};
      event.timestamp = raw_event.common.timestamp;

      if (raw_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        event.type = EventType::WindowResized;
        event.resize.window_id = raw_event.window.windowID;
        event.resize.size.w = raw_event.window.data1;
        event.resize.size.h = raw_event.window.data2;
      } else {
        event.window.window_id = raw_event.window.windowID;

        switch (raw_event.window.event) {
          case SDL_WINDOWEVENT_SHOWN:
            event.type = EventType::WindowShown;
            break;
          case SDL_WINDOWEVENT_HIDDEN:
            event.type = EventType::WindowHidden;
            break;
          case SDL_WINDOWEVENT_EXPOSED:
            event.type = EventType::WindowExposed;
            break;
          case SDL_WINDOWEVENT_MINIMIZED:
            event.type = EventType::WindowMinimized;
            break;
          case SDL_WINDOWEVENT_MAXIMIZED:
            event.type = EventType::WindowMaximized;
            break;
          case SDL_WINDOWEVENT_RESTORED:
            event.type = EventType::WindowRestored;
            break;
          case SDL_WINDOWEVENT_ENTER:
            event.type = EventType::MouseFocusGained;
            break;
          case SDL_WINDOWEVENT_LEAVE:
            event.type = EventType::MouseFocusLost;
            break;
          case SDL_WINDOWEVENT_FOCUS_GAINED:
            event.type = EventType::KeyboardFocusGained;
            break;
          case SDL_WINDOWEVENT_FOCUS_LOST:
            event.type = EventType::KeyboardFocusLost;
            break;
          case SDL_WINDOWEVENT_CLOSE:
            event.type = EventType::WindowClosed;
            break;
          default:
            return std::nullopt;
        }
      }

      return event;
    }

    std::optional<Event> to_event(const SDL_Event& raw_event)
    {
      Event event = {};
      event.timestamp = raw_event.common.timestamp;

      switch (raw_event.type) {
        case SDL_WINDOWEVENT:
          return to_window_event(raw_event);

        case SDL_QUIT:
          event.type = EventType::Quit;
          break;

        case SDL_KEYDOWN:
          assert(raw_event.key.state == SDL_PRESSED);

          if (raw_event.key.repeat == 0) {
            event.type = EventType::KeyPressed;
          } else {
            event.type = EventType::KeyRepeated;
          }

          event.key.window_id = raw_event.key.windowID;
          event.key.keycode = static_cast<Keycode>(raw_event.key.keysym.sym);
          event.key.scancode = static_cast<Scancode>(raw_event.key.keysym.scancode);
          event.key.modifiers = static_cast<Modifier>(raw_event.key.keysym.mod);
          event.key.repeat = raw_event.key.repeat;
          event.key.pressed = true;
          break;

        case SDL_KEYUP:
          assert(raw_event.key.state == SDL_RELEASED);
          event.type = EventType::KeyReleased;
          event.key.window_id = raw_event.key.windowID;
          event.key.keycode = static_cast<Keycode>(raw_event.key.keysym.sym);
          event.key.scancode = static_cast<Scancode>(raw_event.key.keysym.scancode);
          event.key.modifiers = static_cast<Modifier>(raw_event.key.keysym.mod);
          event.key.repeat = raw_event.key.repeat;
          event.key.pressed = false;
          break;

        case SDL_MOUSEWHEEL:
          event.type = EventType::MouseWheelScrolled;
          event.mouse_wheel.window_id = raw_event.wheel.windowID;
          event.mouse_wheel.mouse_id = static_cast<MouseId>(raw_event.wheel.which);
          event.mouse_wheel.offset.x = raw_event.wheel.x;
          event.mouse_wheel.offset.y = raw_event.wheel.y;

          if (raw_event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
            event.mouse_wheel.offset = -event.mouse_wheel.offset;
          }
          break;

        case SDL_MOUSEBUTTONDOWN:
          assert(raw_event.button.state == SDL_PRESSED);
          event.type = EventType::MouseButtonPressed;
          event.mouse_button.window_id = raw_event.button.windowID;
          event.mouse_button.mouse_id = static_cast<MouseId>(raw_event.button.which);
          event.mouse_button.button = MouseButton{ raw_event.button.button };
          event.mouse_button.coordinates.x = raw_event.button.x;
          event.mouse_button.coordinates.y = raw_event.button.y;
          event.mouse_button.clicks = raw_event.button.clicks;
          event.mouse_button.pressed = true;
          break;

        case SDL_MOUSEBUTTONUP:
          assert(raw_event.button.state == SDL_RELEASED);
          event.type = EventType::MouseButtonReleased;
          event.mouse_button.window_id = raw_event.button.windowID;
          event.mouse_button.mouse_id = static_cast<MouseId>(raw_event.button.which);
          event.mouse_button.button = static_cast<MouseButton>(raw_event.button.button);
          event.mouse_button.coordinates.x = raw_event.button.x;
          event.mouse_button.coordinates.y = raw_event.button.y;
          event.mouse_button.clicks = raw_event.button.clicks;
          event.mouse_button.pressed = false;
          break;

        case SDL_MOUSEMOTION:
          event.type = EventType::MouseMoved;
          event.mouse_motion.window_id = raw_event.motion.windowID;
          event.mouse_motion.mouse_id = static_cast<MouseId>(raw_event.motion.which);
          event.mouse_motion.coordinates.x = raw_event.motion.x;
          event.mouse_motion.coordinates.y = raw_event.motion.y;
          event.mouse_motion.motion.x = raw_event.motion.xrel;
          event.mouse_motion.motion.y = raw_event.motion.yrel;
          break;

        case SDL_CONTROLLERDEVICEADDED:
          assert(SDL_IsGameController(raw_event.cdevice.which));
          event.type = EventType::GamepadConnected;
          event.gamepad_connection.gamepad_hw_id = static_cast<GamepadHwId>(raw_event.cdevice.which);
          break;

        case SDL_CONTROLLERDEVICEREMOVED:
          event.type = EventType::GamepadDisconnected;
          event.gamepad_disconnection.gamepad_id = static_cast<GamepadId>(raw_event.cdevice.which);
          break;

        case SDL_CONTROLLERBUTTONDOWN:
          assert(raw_event.cbutton.state == SDL_PRESSED);
          event.type = EventType::GamepadButtonPressed;
          event.gamepad_button.gamepad_id = static_cast<GamepadId>(raw_event.cbutton.which);
          event.gamepad_button.button = static_cast<GamepadButton>(raw_event.cbutton.button);
          event.gamepad_button.pressed = true;
          break;

        case SDL_CONTROLLERBUTTONUP:
          assert(raw_event.cbutton.state == SDL_RELEASED);
          event.type = EventType::GamepadButtonReleased;
          event.gamepad_button.gamepad_id = static_cast<GamepadId>(raw_event.cbutton.which);
          event.gamepad_button.button = static_cast<GamepadButton>(raw_event.cbutton.button);
          event.gamepad_button.pressed = false;
          break;

        case SDL_CONTROLLERAXISMOTION:
          event.type = EventType::GamepadAxisMoved;
          event.gamepad_axis.gamepad_id = static_cast<GamepadId>(raw_event.caxis.which);
          event.gamepad_axis.axis = static_cast<GamepadAxis>(raw_event.caxis.axis);
          event.gamepad_axis.value = raw_event.caxis.value;
          break;

        case SDL_TEXTINPUT:
          event.type = EventType::TextEntered;
          event.text.window_id = raw_event.text.windowID;
          static_assert(std::tuple_size_v<decltype(event.text.text)> == std::extent_v<decltype(raw_event.text.text)>, "Buffer size mismatch.");
          std::copy_n(std::begin(raw_event.text.text), event.text.text.size(), event.text.text.data());
          break;

        case SDL_FINGERDOWN:
          event.type = EventType::TouchBegan;
          event.touch.window_id = raw_event.tfinger.windowID;
          event.touch.touch_id = static_cast<TouchId>(raw_event.tfinger.touchId);
          event.touch.finger = static_cast<FingerId>(raw_event.tfinger.fingerId);
          event.touch.coordinates.x = raw_event.tfinger.x;
          event.touch.coordinates.y = raw_event.tfinger.y;
          event.touch.motion.x = raw_event.tfinger.dx;
          event.touch.motion.y = raw_event.tfinger.dy;
          event.touch.pressure = raw_event.tfinger.pressure;
          break;

        case SDL_FINGERMOTION:
          event.type = EventType::TouchMoved;
          event.touch.window_id = raw_event.tfinger.windowID;
          event.touch.touch_id = static_cast<TouchId>(raw_event.tfinger.touchId);
          event.touch.finger = static_cast<FingerId>(raw_event.tfinger.fingerId);
          event.touch.coordinates.x = raw_event.tfinger.x;
          event.touch.coordinates.y = raw_event.tfinger.y;
          event.touch.motion.x = raw_event.tfinger.dx;
          event.touch.motion.y = raw_event.tfinger.dy;
          event.touch.pressure = raw_event.tfinger.pressure;
          break;

        case SDL_FINGERUP:
          event.type = EventType::TouchEnded;
          event.touch.window_id = raw_event.tfinger.windowID;
          event.touch.touch_id = static_cast<TouchId>(raw_event.tfinger.touchId);
          event.touch.finger = static_cast<FingerId>(raw_event.tfinger.fingerId);
          event.touch.coordinates.x = raw_event.tfinger.x;
          event.touch.coordinates.y = raw_event.tfinger.y;
          event.touch.motion.x = raw_event.tfinger.dx;
          event.touch.motion.y = raw_event.tfinger.dy;
          event.touch.pressure = raw_event.tfinger.pressure;
          break;

        default:
          return std::nullopt;
      };

      return event;
    }

  } // namespace

  std::optional<Event> Event::poll()
  {
    for (;;) {
      SDL_Event raw_event;
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
      SDL_Event raw_event;
      const int status = SDL_WaitEvent(&raw_event);

      if (status == 0) {
        Log::fatal("Error when waiting an event: '{}'", SDL_GetError());
      }

      auto maybe_event = to_event(raw_event);

      if (maybe_event) {
        return *maybe_event;
      }
    }

    assert(false);
  }

} // namespace gf
