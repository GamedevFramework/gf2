#ifndef HOME_GAME_HUB_H
#define HOME_GAME_HUB_H

#include <memory>

#include <gf2/core/FontManager.h>
#include <gf2/core/Random.h>

#include <gf2/audio/AudioManager.h>
#include <gf2/framework/SceneSystem.h>
#include <gf2/graphics/RenderAsync.h>
#include <gf2/graphics/SceneManager.h>

#include "SplashResources.h"
#include "SplashScene.h"
#include "WorldData.h"
#include "WorldScene.h"

namespace home {

  class GameHub : public gf::SceneSystem {
  public:
    GameHub(const std::filesystem::path& asset_directory);

    SplashScene* splash_scene()
    {
      return m_splash_scene.get();
    }

    bool is_world_loaded();

    WorldScene* world_scene()
    {
      return m_world_scene.get();
    }

  private:
    SplashResources m_splash_data;
    std::unique_ptr<SplashScene> m_splash_scene = nullptr;

    gf::RenderAsync m_world_async;
    WorldData m_world_data;
    std::unique_ptr<WorldScene> m_world_scene = nullptr;
  };

}

#endif // HOME_GAME_HUB_H
