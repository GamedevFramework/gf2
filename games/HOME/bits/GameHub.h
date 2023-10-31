#ifndef HOME_GAME_HUB_H
#define HOME_GAME_HUB_H

#include <gf2/core/FontManager.h>

#include <gf2/audio/AudioManager.h>
#include <gf2/graphics/SceneManager.h>

#include "ResourceHub.h"
#include "WorldScene.h"

namespace home {

  struct GameHub : gf::SingleSceneManager {
    GameHub(const std::filesystem::path& asset_directory);

    gf::FontManager font_manager;
    gf::AudioManager audio_manager;

    ResourceHub resource_hub;

    WorldScene world_scene;
  };

}

#endif // HOME_GAME_HUB_H
