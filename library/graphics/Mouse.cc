// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Mouse.h>

#include <memory>
#include <type_traits>

#include <SDL3/SDL.h>

#include <gf2/core/Log.h>
#include <gf2/core/MouseTypes.h>

namespace gf {

  namespace {

    static_assert(std::is_same_v<std::underlying_type_t<MouseId>, SDL_MouseID>);
    static_assert(std::is_same_v<std::underlying_type_t<MouseWheelDirection>, std::make_signed_t<SDL_MouseWheelDirection>>);
    static_assert(TouchMouseId == static_cast<MouseId>(SDL_TOUCH_MOUSEID));

    template<MouseButton Button, uint8_t Value>
    constexpr void mouse_button_check()
    {
      static_assert(static_cast<uint8_t>(Button) == Value);
    }

    [[maybe_unused]] constexpr void mouse_check()
    {
      mouse_button_check<MouseButton::Left, SDL_BUTTON_LEFT>();
      mouse_button_check<MouseButton::Middle, SDL_BUTTON_MIDDLE>();
      mouse_button_check<MouseButton::Right, SDL_BUTTON_RIGHT>();
      mouse_button_check<MouseButton::XButton1, SDL_BUTTON_X1>();
      mouse_button_check<MouseButton::XButton2, SDL_BUTTON_X2>();
    }

    struct MouseIdDeleter {
      void operator()(SDL_MouseID* ids)
      {
        SDL_free(ids); // NOLINT(cppcoreguidelines-no-malloc)
      }
    };

  } // namespace

  bool Mouse::connected()
  {
    return SDL_HasMouse();
  }

  std::vector<MouseId> Mouse::devices()
  {
    int count = 0;
    const std::unique_ptr<SDL_MouseID[], MouseIdDeleter> raw_devices(SDL_GetMice(&count));

    std::vector<MouseId> devices;
    devices.reserve(count);

    for (int i = 0; i < count; ++i) {
      devices.push_back(MouseId{raw_devices[i]});
    }

    return devices;
  }

  ZString Mouse::name(MouseId id)
  {
    if (const char* name = SDL_GetMouseNameForID(static_cast<SDL_MouseID>(id)); name != nullptr) {
      return name;
    }

    Log::error("Failed to get mouse name: {}", SDL_GetError());
    return "";
  }

  void Mouse::capture(bool captured)
  {
    if (!SDL_CaptureMouse(captured)) {
      Log::error("Failed to capture the mouse: {}", SDL_GetError());
    }
  }

} // namespace gf
