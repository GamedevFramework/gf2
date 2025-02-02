// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/GamePaths.h>

#include <memory>

#include <SDL2/SDL.h>

namespace gf {

  namespace {

    struct StringDeleter {
      void operator()(char* str) const noexcept
      {
        SDL_free(str);
      }
    };

  }

  std::filesystem::path user_data_path(const std::string& organization, const std::string& application)
  {
    std::unique_ptr<char[], StringDeleter> pref_path(SDL_GetPrefPath(organization.c_str(), application.c_str()));
    return pref_path.get();
  }

  std::filesystem::path application_base_path()
  {
    std::unique_ptr<char[], StringDeleter> base_path(SDL_GetBasePath());
    return base_path.get();
  }

}
