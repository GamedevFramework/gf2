#ifndef HOME_GAME_HUB_H
#define HOME_GAME_HUB_H

#include <memory>

#include <gf2/core/FontManager.h>

#include <gf2/audio/AudioManager.h>
#include <gf2/graphics/SceneManager.h>
#include <gf2/physics/PhysicsWorld.h>

#include "ResourceHub.h"
#include "SplashData.h"
#include "SplashScene.h"
#include "WorldData.h"
#include "WorldScene.h"

namespace home {

  class GameHub : public gf::SceneManager {
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

    gf::PhysicsWorld* physics_world()
    {
      return &m_physics_world;
    }

    SplashScene* splash_scene()
    {
      return m_splash_scene.get();
    }

    WorldScene* world_scene()
    {
      return m_world_scene.get();
    }

  private:
    gf::FontManager m_font_manager;
    gf::AudioManager m_audio_manager;
    gf::PhysicsWorld m_physics_world;

    ResourceHub m_resource_hub;

    SplashData m_splash_data;
    std::unique_ptr<SplashScene> m_splash_scene = nullptr;

    WorldData m_world_data;
    std::unique_ptr<WorldScene> m_world_scene = nullptr;
  };

}

#endif // HOME_GAME_HUB_H
