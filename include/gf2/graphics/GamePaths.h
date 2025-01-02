// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GAME_PATH_H
#define GF_GAME_PATH_H

#include <filesystem>
#include <string>

#include "GraphicsApi.h"

namespace gf {

  GF_GRAPHICS_API std::filesystem::path user_data_path(const std::string& organization, const std::string& application);
  GF_GRAPHICS_API std::filesystem::path application_base_path();

}

#endif // GF_GAME_PATH_H
