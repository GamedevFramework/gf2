#ifndef HOME_WORLD_SCENE_H
#define HOME_WORLD_SCENE_H

#include <gf2/core/ResourceBundle.h>

#include <gf2/audio/Music.h>
#include <gf2/audio/Sound.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/TextEntity.h>

#include "HeroEntity.h"
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

    gf::Music* main_theme_music = nullptr;

    gf::Sound* mining_sound = nullptr;
    gf::Sound* o2_filling_sound = nullptr;
    gf::Sound* breath_low_o2_sound = nullptr;
    gf::Sound* victory_sound = nullptr;
    gf::Sound* death_sound = nullptr;

    HeroEntity m_hero_entity;
  };

}

#endif // HOME_WORLD_SCENE_H
