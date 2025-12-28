// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Clipboard.h>

#include <memory>

#include <SDL3/SDL.h>

#include <gf2/core/Log.h>

namespace gf {

  namespace {

    struct ClipboardStringDeleter {
      void operator()(char* str) const noexcept
      {
        SDL_free(str); // NOLINT(cppcoreguidelines-no-malloc)
      }
    };

  }

  std::string Clipboard::text()
  {
    if (!SDL_HasClipboardText()) {
      return "";
    }

    const std::unique_ptr<char[], ClipboardStringDeleter> raw_text(SDL_GetClipboardText());

    if (raw_text == nullptr) {
      Log::error("Unable to get clipboard text: {}.", SDL_GetError());
      return "";
    }

    return raw_text.get();
  }

  void Clipboard::set_text(const std::string& text)
  {
    SDL_SetClipboardText(text.c_str());
  }

}
