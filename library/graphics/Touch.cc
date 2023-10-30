// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/core/TouchTypes.h>
// clang-format on

#include <type_traits>
#include <utility>

#include <SDL2/SDL.h>

namespace gf {

  namespace {

    [[maybe_unused]] constexpr void touch_check()
    {
      static_assert(std::is_same_v<std::underlying_type_t<TouchId>, decltype(SDL_MOUSE_TOUCHID)>, "Check gf::Touch");
      static_assert(MouseTouchId == static_cast<TouchId>(SDL_MOUSE_TOUCHID), "Check gf::Touch");

      static_assert(std::is_same_v<std::underlying_type_t<FingerId>, decltype(std::declval<SDL_Finger>().id)>, "Check gf::Touch");
    }

  } // namespace

} // namespace gf
