#ifndef HOME_BACKPACK_ENTITY_H
#define HOME_BACKPACK_ENTITY_H

#include <gf2/core/Signal.h>

#include <gf2/graphics/Entity.h>
#include <gf2/graphics/Shape.h>
#include <gf2/graphics/SpriteEntity.h>

#include "SupplyType.h"
#include "WorldResources.h"

namespace home {

  class GameHub;

  class BackpackEntity : public gf::HudEntity {
  public:
    BackpackEntity(GameHub* hub, const WorldResources& resources);

    void set_hero_location(gf::Vec2F location);

    void update(gf::Time time) override;
    void render(gf::RenderRecorder& recorder) override;

    void put_in_backpack(SupplyType type, int32_t quantity);

    gf::Signal<void(SupplyType type, int32_t quantity)> unload; // NOLINT

  private:
    GameHub* m_hub = nullptr;
    gf::RectF m_ship_bounds = {};
    gf::Vec2F m_hero_location = { 0.0f, 0.0f };

    struct PackedSupply {
      SupplyType type;
      int32_t quantity;
    };

    std::vector<PackedSupply> m_supplies;
    int32_t m_total_quantity = 0;
    int32_t m_oxygen_quantity = 0;

    gf::SpriteEntity m_backpack_sprite;
    gf::SpriteEntity m_oxygen_sprite;
    gf::ShapeGroup m_shapes;
  };

}

#endif // HOME_BACKPACK_ENTITY_H
