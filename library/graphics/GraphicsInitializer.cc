// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/GraphicsInitializer.h>

#include <atomic>

#include <SDL2/SDL.h>
#include <volk.h>

#include <gf2/core/Log.h>

#include <gf2/graphics/Gamepad.h>

namespace gf {

  namespace {
    std::atomic_bool g_graphics_loaded = false; // NOLINT
  }

  GraphicsInitializer::GraphicsInitializer()
  {
    if (!g_graphics_loaded.exchange(true)) { // we are the first
      // initialize SDL

      if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
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

      // initialize volk

      if (volkInitialize() != VK_SUCCESS) {
        Log::error("Unable to initialize volk.");
        return;
      }

    } else {
      Log::warning("You are trying to load SDL multiple times. Do not do that!");
    }
  }

  GraphicsInitializer::~GraphicsInitializer()
  {
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
    g_graphics_loaded = false;

    if (SDL_WasInit(0) == 0) {
      SDL_Quit();
    }
  }

} // namespace gf
