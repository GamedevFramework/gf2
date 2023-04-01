// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Mouse.h>
// clang-format on

#include <SDL2/SDL.h>

namespace gf {

#define MOUSE_CHECK(GF_VAL, SDL_VAL) static_assert(static_cast<uint32_t>(GF_VAL) == (SDL_VAL), "Problem with " #SDL_VAL)

  MOUSE_CHECK(MouseButton::Left, SDL_BUTTON_LEFT);
  MOUSE_CHECK(MouseButton::Middle, SDL_BUTTON_MIDDLE);
  MOUSE_CHECK(MouseButton::Right, SDL_BUTTON_RIGHT);
  MOUSE_CHECK(MouseButton::XButton1, SDL_BUTTON_X1);
  MOUSE_CHECK(MouseButton::XButton2, SDL_BUTTON_X2);

#undef MOUSE_CHECK

} // namespace gf
