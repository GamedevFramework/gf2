#ifndef HOME_WORLD_SCENE_H
#define HOME_WORLD_SCENE_H

#include <gf2/core/ResourceBundle.h>

#include <gf2/graphics/Scene.h>
#include <gf2/graphics/TextEntity.h>

#include "WorldData.h"

namespace home {
  class GameHub;

  class WorldScene : public gf::StandardScene {
  public:
    WorldScene(GameHub* hub, const WorldData& data);

  private:
    gf::TextEntity m_title;
  };

}

#endif // HOME_WORLD_SCENE_H
