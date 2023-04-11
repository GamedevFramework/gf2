// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Initializer.h>
// clang-format on

#include <atomic>

#include <SDL2/SDL.h>

namespace gf {

  namespace {
    std::atomic_int g_loaded = 0; // NOLINT
  }

  Initializer::Initializer()
  {
    g_loaded.fetch_add(1);
  }

  Initializer::~Initializer()
  {
    if (g_loaded.fetch_sub(1) == 1) { // we are the last
      SDL_Quit();
    }
  }

  Initializer::Initializer(Initializer&& /* unused */) noexcept
  {
    g_loaded.fetch_add(1);
  }

  Initializer& Initializer::operator=(Initializer&& /* unused */) noexcept
  {
    g_loaded.fetch_add(1);
    return *this;
  }

} // namespace gf
