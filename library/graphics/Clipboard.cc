// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Clipboard.h>

#include <SDL2/SDL.h>

#include <gf2/core/Log.h>

namespace gf {

  std::string Clipboard::text()
  {
    if (SDL_HasClipboardText() == SDL_FALSE) {
      return "";
    }

    char* raw_text = SDL_GetClipboardText();

    if (raw_text == nullptr) {
      Log::error("Unable to get clipboard text: {}.", SDL_GetError());
      return "";
    }

    std::string result(raw_text);
    SDL_free(raw_text);
    return result;
  }

  void Clipboard::set_text(const std::string& text)
  {
    SDL_SetClipboardText(text.c_str());
  }

}
