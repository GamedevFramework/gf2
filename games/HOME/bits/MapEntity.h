#ifndef HOME_MAP_ENTITY_H
#define HOME_MAP_ENTITY_H

#include <gf2/core/TiledMapData.h>
#include <gf2/graphics/Entity.h>
#include <gf2/graphics/TiledMap.h>

#include "WorldData.h"

namespace home {
  class GameHub;

  class MapEntity : public gf::Entity {
  public:
    MapEntity(GameHub* hub, const WorldData& data);

  private:
    gf::TiledMap* m_map;
  };

}

#endif // HOME_MAP_ENTITY_H
