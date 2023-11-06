#include "WorldScene.h"

#include "GameHub.h"

namespace home {

  WorldScene::WorldScene(GameHub* hub, const WorldData& data)
  {
    set_clear_color(gf::Color(0xAEF6B8));
    set_world_center({ 0.0f, 0.0f });
    set_world_size({ 800.0f, 800.0f });
  }

}
