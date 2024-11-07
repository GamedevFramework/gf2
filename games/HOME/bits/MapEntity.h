#ifndef HOME_MAP_ENTITY_H
#define HOME_MAP_ENTITY_H

#include <gf2/graphics/Entity.h>
#include <gf2/graphics/RichMapRenderer.h>
#include <gf2/physics/PhysicsWorld.h>

#include "WorldResources.h"

namespace home {
  class GameHub;

  class MapEntity : public gf::Entity {
  public:
    MapEntity(GameHub* hub, const WorldResources& resources, gf::PhysicsWorld* physics_world);

    void set_hero_location(gf::Vec2F location);

    void render(gf::RenderRecorder& recorder) override;

  private:
    gf::RichMapRenderer m_map_renderer;
    gf::Vec2F m_location = { 0.0f, 0.0f };
  };

}

#endif // HOME_MAP_ENTITY_H
