#ifndef HOME_GAME_HUB_H
#define HOME_GAME_HUB_H

#include <memory>

#include <gf2/core/FontManager.h>

#include <gf2/audio/AudioManager.h>
#include <gf2/graphics/SceneManager.h>

#include "ResourceHub.h"
#include "WorldScene.h"

namespace home {

  class GameHub : public gf::SingleSceneManager {
  public:
    GameHub(const std::filesystem::path& asset_directory);

    gf::ResourceManager* resource_manager()
    {
      return &m_resource_hub.manager;
    }

    gf::FontManager* font_manager()
    {
      return &m_font_manager;
    }

    gf::AudioManager* audio_manager()
    {
      return &m_audio_manager;
    }

    WorldScene* world_scene()
    {
      return m_world_scene.get();
    }

  private:
    gf::FontManager m_font_manager;
    gf::AudioManager m_audio_manager;

    ResourceHub m_resource_hub;

    std::unique_ptr<WorldScene> m_world_scene = nullptr;
  };

}

#endif // HOME_GAME_HUB_H
