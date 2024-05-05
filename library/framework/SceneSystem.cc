// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/framework/SceneSystem.h>

namespace gf {

  SceneSystem::SceneSystem(const std::string& title, Vec2I size, const std::filesystem::path& asset_directory)
  : SceneManager(title, size)
  , m_resource_system(asset_directory)
  {
  }

}
