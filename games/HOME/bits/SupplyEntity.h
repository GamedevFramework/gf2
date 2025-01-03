#ifndef HOME_SUPPLY_ENTITY_H
#define HOME_SUPPLY_ENTITY_H

#include <gf2/core/Random.h>
#include <gf2/core/Signal.h>
#include <gf2/core/Transform.h>
#include <gf2/core/Vec2.h>

#include <gf2/audio/Sound.h>
#include <gf2/graphics/Entity.h>
#include <gf2/graphics/Shape.h>
#include <gf2/graphics/SpriteEntity.h>

#include "SupplyType.h"
#include "WorldResources.h"

namespace home {
  class GameHub;

  struct Supply {
    Supply(SupplyType type, gf::RectF bounds, gf::Random* random);

    SupplyType type;
    int32_t initial_quantity = 0;
    int32_t current_quantity = 0;
    gf::RectF bounds;
  };

  class SupplyEntity : public gf::Entity {
  public:
    SupplyEntity(GameHub* hub, const WorldResources& resources);

    void update(gf::Time time) override;
    void render(gf::RenderRecorder& recorder) override;

    void set_hero_location(gf::Vec2F location)
    {
      m_hero_location = location;
    }

    gf::Signal<void(SupplyType, int32_t)> harvest; // NOLINT

  private:
    GameHub* m_hub = nullptr;

    gf::Sound* m_oxygen_sound = nullptr;
    gf::Sound* m_mining_sound = nullptr;

    gf::SpriteEntity m_energy_sprite;
    gf::SpriteEntity m_metal_sprite;
    gf::SpriteEntity m_oxygen_sprite;

    gf::Vec2F m_hero_location = { 0.0f, 0.0f };
    std::vector<Supply> m_supplies;

    gf::ShapeGroup m_shapes;
  };

}

#endif // HOME_SUPPLY_ENTITY_H
