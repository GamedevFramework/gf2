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
    std::atomic_int g_loaded = 0; // NOLINT
  }

  GraphicsInitializer::GraphicsInitializer()
  {
    if (g_loaded.fetch_add(1) == 0) { // we are the first
      // initialize SDL

      if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) != 0) {
        Log::error("Unable to initialize SDL: '%s'\n", SDL_GetError());
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
    }
  }

  GraphicsInitializer::~GraphicsInitializer()
  {
    if (g_loaded.fetch_sub(1) == 1) { // we are the last
      SDL_Quit();
    }
  }

  GraphicsInitializer::GraphicsInitializer(GraphicsInitializer&& /* unused */) noexcept
  {
    g_loaded.fetch_add(1);
  }

  GraphicsInitializer& GraphicsInitializer::operator=(GraphicsInitializer&& /* unused */) noexcept
  {
    g_loaded.fetch_add(1);
    return *this;
  }

  bool GraphicsInitializer::initialized()
  {
    return g_loaded > 0;
  }

} // namespace gf
