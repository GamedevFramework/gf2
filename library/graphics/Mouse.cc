// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <type_traits>

#include <SDL2/SDL.h>

#include <gf2/core/MouseTypes.h>

namespace gf {

  namespace {

    template<MouseButton Button, uint32_t Value>
    constexpr void mouse_button_check()
    {
      static_assert(static_cast<uint32_t>(Button) == Value, "Check gf::Mouse");
    }

    [[maybe_unused]] constexpr void mouse_check()
    {
      mouse_button_check<MouseButton::Left, SDL_BUTTON_LEFT>();
      mouse_button_check<MouseButton::Middle, SDL_BUTTON_MIDDLE>();
      mouse_button_check<MouseButton::Right, SDL_BUTTON_RIGHT>();
      mouse_button_check<MouseButton::XButton1, SDL_BUTTON_X1>();
      mouse_button_check<MouseButton::XButton2, SDL_BUTTON_X2>();

      static_assert(std::is_same_v<std::underlying_type_t<MouseId>, decltype(SDL_TOUCH_MOUSEID)>, "Check gf::Mouse");
      static_assert(TouchMouseId == static_cast<MouseId>(SDL_TOUCH_MOUSEID), "Check gf::Mouse");
    }

  } // namespace

} // namespace gf
