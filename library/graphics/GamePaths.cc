// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/GamePaths.h>

#include <SDL2/SDL.h>

namespace gf {

  std::filesystem::path user_data_path(const std::string& organization, const std::string& application)
  {
    char* pref_path = SDL_GetPrefPath(organization.c_str(), application.c_str());

    if (pref_path == nullptr) {
      return {};
    }

    std::filesystem::path path(pref_path);
    SDL_free(pref_path);
    return path;
  }

  std::filesystem::path application_base_path()
  {
    char* base_path = SDL_GetBasePath();

    if (base_path == nullptr) {
      return {};
    }

    std::filesystem::path path(base_path);
    SDL_free(base_path);
    return path;
  }

}
