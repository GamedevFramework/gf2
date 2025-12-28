#include "SupplyEntity.h"

#include <algorithm>

#include <gf2/core/Color.h>
#include <gf2/core/Math.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>
#include <gf2/graphics/SpriteEntity.h>

#include "GameHub.h"

namespace home {

  namespace {
    constexpr int HarvestSpeed = 100'000;
    constexpr int HarvestQuantityMin = 700'000;
    constexpr int HarvestQuantityMax = 1'300'000;

    constexpr int OxygenHarvestSpeed = 6'000;
    constexpr int OxygenQuantityMin = 30'000;
    constexpr int OxygenQuantityMax = 70'000;

    constexpr float SupplyDistance = 64.0f;

    int32_t compute_initial_quantity(SupplyType type, gf::Random* random)
    {
      if (type == SupplyType::Oxygen) {
        return random->compute_uniform_integer(OxygenQuantityMin, OxygenQuantityMax);
      }

      return random->compute_uniform_integer(HarvestQuantityMin, HarvestQuantityMax);
    }

  }

  Supply::Supply(SupplyType type, gf::RectF bounds, gf::Random* random)
  : type(type)
  , initial_quantity(compute_initial_quantity(type, random))
  , current_quantity(initial_quantity)
  , bounds(bounds)
  {
  }

  SupplyEntity::SupplyEntity(GameHub* hub, const WorldResources& resources)
  : m_hub(hub)
  , m_oxygen_sound(hub->resource_manager()->get<gf::Sound>(resources.oxygen_sound.filename))
  , m_mining_sound(hub->resource_manager()->get<gf::Sound>(resources.mining_sound.filename))
  , m_energy_sprite(resources.energy_sprite, hub->render_manager(), hub->resource_manager())
  , m_metal_sprite(resources.metal_sprite, hub->render_manager(), hub->resource_manager())
  , m_oxygen_sprite(resources.oxygen_sprite, hub->render_manager(), hub->resource_manager())
  {
    const gf::RichMap* rich_map = hub->resource_manager()->get<gf::RichMap>(resources.map.filename);
    const gf::TiledMap* tiled_map = rich_map->tiled_map();

    for (const auto& object_layer : tiled_map->object_layers) {
      if (object_layer.layer.name != "Resources") {
        continue;
      }

      for (const auto& object : object_layer.objects) {
        if (object.type != gf::MapObjectType::Tile) {
          continue;
        }

        const gf::MapTile tile = std::get<gf::MapTile>(object.feature);
        const gf::MapTileset* tileset = tiled_map->tileset_from_gid(tile.gid);
        assert(tileset != nullptr);
        const uint32_t gid = tile.gid - tileset->first_gid;
        assert(gid < 3);

        const gf::RectF bounds = gf::RectF::from_bottom_left_size(object.location + object_layer.layer.offset, tileset->tile_size);

        m_supplies.emplace_back(static_cast<SupplyType>(gid), bounds, hub->random());
      }
    }
  }

  void SupplyEntity::update(gf::Time time)
  {
    bool is_mining = false;

    for (auto& supply : m_supplies) {
      if (gf::square_distance(m_hero_location, supply.bounds.center()) > gf::square(SupplyDistance)) {
        continue;
      }

      is_mining = true;
      int quantity = 0;

      if (supply.type == SupplyType::Oxygen) {
        quantity = static_cast<int>(OxygenHarvestSpeed * time.as_seconds());

        if (!m_oxygen_sound->playing()) {
          m_oxygen_sound->start();
        }
      } else {
        quantity = static_cast<int>(HarvestSpeed * time.as_seconds());

        if (!m_mining_sound->playing()) {
          m_mining_sound->start();
        }
      }

      supply.current_quantity -= quantity;

      harvest.emit(supply.type, quantity);
    }

    if (!is_mining) {
      m_oxygen_sound->stop();
      m_mining_sound->stop();
    }

    std::erase_if(m_supplies, [](const Supply& supply) { return supply.current_quantity <= 0; });

    if (is_mining) {
      gf::ShapeGroupBuffer shape_group_buffer;

      for (const auto& supply : m_supplies) {
        if (supply.current_quantity == supply.initial_quantity) {
          continue;
        }

        auto bounds = supply.bounds;
        bounds.extent.h = 10.0f;
        auto color = to_color(supply.type);

        gf::ShapeBuffer back_shape_buffer = gf::ShapeBuffer::make_rectangle(bounds);
        back_shape_buffer.color = gf::darker(color) * gf::opaque(0.3f);
        back_shape_buffer.outline_color = gf::darker(color);
        back_shape_buffer.outline_thickness = 2.0f;
        shape_group_buffer.shapes.emplace_back(std::move(back_shape_buffer));

        const float percent = static_cast<float>(supply.current_quantity) / static_cast<float>(supply.initial_quantity);
        bounds.extent.w *= percent;

        gf::ShapeBuffer front_shape_buffer = gf::ShapeBuffer::make_rectangle(bounds);
        front_shape_buffer.color = color;
        shape_group_buffer.shapes.emplace_back(std::move(front_shape_buffer));
      }

      m_shapes.update(shape_group_buffer, m_hub->render_manager());
    }
  }

  void SupplyEntity::render(gf::RenderRecorder& recorder)
  {
    // sprites

    for (auto& supply : m_supplies) {
      gf::SpriteEntity* entity = nullptr;

      switch (supply.type) {
        case SupplyType::Metal:
          entity = &m_metal_sprite;
          break;
        case SupplyType::Energy:
          entity = &m_energy_sprite;
          break;
        case SupplyType::Oxygen:
          entity = &m_oxygen_sprite;
          break;
      }

      assert(entity != nullptr);
      entity->set_location(supply.bounds.position());
      entity->render(recorder);
    }

    // life bars

    auto geometry = m_shapes.geometry();

    if (geometry.count > 0) {
      gf::RenderObject object = {};
      object.priority = priority();
      object.geometry = geometry;
      recorder.record(object);
    }
  }

}
