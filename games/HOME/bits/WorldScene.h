#ifndef HOME_WORLD_SCENE_H
#define HOME_WORLD_SCENE_H

#include <gf2/core/ResourceBundle.h>

#include <gf2/audio/Music.h>
#include <gf2/audio/Sound.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/TextEntity.h>

#include "HeroEntity.h"
#include "MapEntity.h"
#include "PhysicsDebug.h"
#include "WorldData.h"

namespace home {
  class GameHub;

  class WorldScene : public gf::StandardScene {
  public:
    WorldScene(GameHub* hub, const WorldData& data);

  private:
    void do_update(gf::Time time) override;
    void do_process_event(const gf::Event& event) override;

    GameHub* m_hub = nullptr;

    gf::Music* m_main_theme_music = nullptr;

    gf::Sound* m_mining_sound = nullptr;
    gf::Sound* m_o2_filling_sound = nullptr;
    gf::Sound* m_breath_low_o2_sound = nullptr;
    gf::Sound* m_victory_sound = nullptr;
    gf::Sound* m_death_sound = nullptr;

    MapEntity m_map_entity;
    HeroEntity m_hero_entity;

    PhysicsDebug m_physics_debug;
  };

}

#endif // HOME_WORLD_SCENE_H
