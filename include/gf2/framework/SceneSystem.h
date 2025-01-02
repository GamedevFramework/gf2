// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SCENE_SYSTEM_H
#define GF_SCENE_SYSTEM_H

#include <gf2/core/FontManager.h>
#include <gf2/core/Random.h>

#include <gf2/audio/AudioManager.h>
#include <gf2/graphics/SceneManager.h>

#include "FrameworkApi.h"
#include "ResourceSystem.h"

namespace gf {

  class GF_FRAMEWORK_API SceneSystem : public SceneManager {
  public:
    SceneSystem(const std::string& title, Vec2I size, const std::filesystem::path& asset_directory);

    Random* random()
    {
      return &m_random;
    }

    FontManager* font_manager()
    {
      return &m_font_manager;
    }

    AudioManager* audio_manager()
    {
      return &m_audio_manager;
    }

    ResourceSystem* resource_manager()
    {
      return &m_resource_system;
    }

  private:
    Random m_random;
    FontManager m_font_manager;
    AudioManager m_audio_manager;
    ResourceSystem m_resource_system;
  };

}

#endif // GF_SCENE_SYSTEM_H
