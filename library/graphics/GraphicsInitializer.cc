// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/GraphicsInitializer.h>

#include <atomic>
#include <memory>

#include <SDL3/SDL.h>
#include <volk.h>

#include <gf2/core/Log.h>

#include <gf2/graphics/Gamepad.h>

namespace gf {

  namespace {
    std::atomic_bool g_graphics_loaded = false; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

    struct JoystickIdDeleter {
      void operator()(SDL_JoystickID* list)
      {
        SDL_free(list);
      }
    };

  }

  GraphicsInitializer::GraphicsInitializer()
  {
    if (!g_graphics_loaded.exchange(true)) { // we are the first
      // initialize SDL

#ifdef SDL_PLATFORM_LINUX
      SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland,x11");
#endif

      if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        Log::error("Unable to initialize SDL: '{}'", SDL_GetError());
        return;
      }

      SDL_ClearError();

      // add game controller db

      // open already plugged controllers

      if (SDL_HasGamepad()) {
        int count = 0;
        std::unique_ptr<SDL_JoystickID[], JoystickIdDeleter> gamepads(SDL_GetGamepads(&count));

        for (int i = 0; i < count; ++i) {
          Gamepad::open(GamepadId{gamepads[i]});
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
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);
    g_graphics_loaded = false;

    if (SDL_WasInit(0) == 0) {
      SDL_Quit();
    }
  }

} // namespace gf
