#ifndef HOME_WORLD_SCENE_H
#define HOME_WORLD_SCENE_H

#include <gf2/core/ActionGroup.h>
#include <gf2/core/ResourceBundle.h>

#include <gf2/audio/Music.h>
#include <gf2/audio/Sound.h>
#include <gf2/framework/GraphicsPhysicsDebug.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/TextEntity.h>
#include <gf2/physics/PhysicsWorld.h>

#include "BackpackEntity.h"
#include "HeroEntity.h"
#include "MapEntity.h"
#include "SupplyEntity.h"
#include "WorldResources.h"

namespace home {
  class GameHub;

  class WorldScene : public gf::Scene {
  public:
    WorldScene(GameHub* hub, const WorldResources& resources);

    gf::PhysicsWorld* physics_world()
    {
      return &m_physics_world;
    }

  private:
    static gf::ActionGroupSettings compute_settings();

    void do_update(gf::Time time) override;
    void do_process_event(const gf::Event& event) override;
    void do_handle_actions() override;

    GameHub* m_hub = nullptr;

    gf::ActionGroup m_action_group;
    bool m_debug = false;

    gf::Music* m_main_theme_music = nullptr;

    gf::Sound* m_breath_low_o2_sound = nullptr;
    gf::Sound* m_victory_sound = nullptr;
    gf::Sound* m_death_sound = nullptr;

    gf::PhysicsWorld m_physics_world;
    gf::GraphicsPhysicsDebugEntity m_physics_debug;

    MapEntity m_map_entity;
    SupplyEntity m_supply_entity;
    HeroEntity m_hero_entity;

    BackpackEntity m_backpack_entity;
  };

}

#endif // HOME_WORLD_SCENE_H
