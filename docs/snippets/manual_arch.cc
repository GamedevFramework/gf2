#include <memory>

#include <gf2/framework/SceneSystem.h>
#include <gf2/graphics/Scene.h>

struct KickoffResources {
};

struct KickoffAct;

struct WorldResources {
};

struct WorldModel {
};

struct WorldAct;

// tag::game_h[]
class Game : public gf::SceneSystem {
public:
  Game();

  KickoffAct* kickoff_act() { return m_kickoff_act.get(); }

  WorldAct* world_act() { return m_world_act.get(); }

  WorldModel* world_model() { return &m_world_model; }

private:
  KickoffResources m_kickoff_resources;
  std::unique_ptr<KickoffAct> m_kickoff_act;

  WorldResources m_world_resources;
  WorldModel m_world_model;
  std::unique_ptr<WorldAct> m_world_act;
};
// end::game_h[]

// tag::scene_h[]
class MapScene : public gf::Scene {
public:
  MapScene(Game* game, const WorldResources& resources);

};
// end::scene_h[]

class OptionsScene : public gf::Scene {
public:
  OptionsScene(Game* game, const WorldResources& resources);

};

// tag::act_h[]
struct WorldAct {
  WorldAct(Game* game, const WorldResources& resources);

  MapScene map_scene;
  OptionsScene option_scene;
};
// end::act_h[]


