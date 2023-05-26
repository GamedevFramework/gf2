// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/AudioInitializer.h>
// clang-format on

#include <atomic>

#include <SDL2/SDL.h>
#include <gf2/Log.h>

namespace gf {

  namespace {
    std::atomic_bool g_audio_loaded = false; // NOLINT
  }

  AudioInitializer::AudioInitializer()
  {
    if (!g_audio_loaded.exchange(true)) { // we are the first
      // initialize SDL

      if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        Log::error("Unable to initialize SDL: '{}'", SDL_GetError());
        return;
      }
    } else {
      Log::warning("You are trying to load SDL multiple times. Do not do that!");
    }
  }

  AudioInitializer::~AudioInitializer()
  {
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    g_audio_loaded = false;

    if (SDL_WasInit(0) == 0) {
      Log::info("Quitting SDL.");
      SDL_Quit();
    }
  }

} // namespace gf
