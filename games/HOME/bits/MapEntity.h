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

    void set_hero_location(gf::Vec2F location);

    void render(gf::RenderRecorder& recorder) override;

  private:
    gf::TiledMap* m_map;
    gf::Vec2F m_location = { 0.0f, 0.0f };
  };

}

#endif // HOME_MAP_ENTITY_H
