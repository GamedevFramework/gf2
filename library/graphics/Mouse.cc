// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <type_traits>

#include <SDL3/SDL.h>

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

  } // namespace

} // namespace gf
