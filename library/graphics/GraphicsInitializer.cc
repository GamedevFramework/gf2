// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/GraphicsInitializer.h>
// clang-format on

#include <atomic>

#include <SDL2/SDL.h>
#include <gf2/Gamepad.h>
#include <gf2/Log.h>

namespace gf {

  namespace {
    std::atomic_bool g_graphics_loaded = false; // NOLINT
  }

  GraphicsInitializer::GraphicsInitializer()
  {
    if (!g_graphics_loaded.exchange(true)) { // we are the first
      // initialize SDL

      if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) != 0) {
        Log::error("Unable to initialize SDL: '{}'", SDL_GetError());
        return;
      }

      SDL_ClearError();

      // add game controller db

      // open already plugged controllers

      for (int index = 0; index < SDL_NumJoysticks(); ++index) {
        if (SDL_IsGameController(index) == SDL_TRUE) {
          Gamepad::open(static_cast<GamepadHwId>(index));
        }
      }
    } else {
      Log::warning("You are trying to load SDL multiple times. Do not do that!");
    }
  }

} // namespace gf
