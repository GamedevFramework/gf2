#include "SupplyEntity.h"

#include <gf2/core/Color.h>
#include <gf2/core/Math.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

#include "GameHub.h"

#include "gf2/graphics/SpriteEntity.h"

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

  SupplyEntity::SupplyEntity(GameHub* hub, const WorldData& data)
  : m_hub(hub)
  , m_oxygen_sound(hub->resource_manager()->get<gf::Sound>(data.oxygen_sound.filename))
  , m_mining_sound(hub->resource_manager()->get<gf::Sound>(data.mining_sound.filename))
  , m_energy_sprite(data.energy_sprite, hub->render_manager(), hub->resource_manager())
  , m_metal_sprite(data.metal_sprite, hub->render_manager(), hub->resource_manager())
  , m_oxygen_sprite(data.oxygen_sprite, hub->render_manager(), hub->resource_manager())
  {
    const auto* map_data = hub->resource_manager()->get<gf::TiledMapResource>(data.map);

    for (const auto& object_layer : map_data->data.object_layers) {
      if (object_layer.layer.name != "Resources") {
        continue;
      }

      for (const auto& object_data : object_layer.objects) {
        if (object_data.type != gf::ObjectType::Tile) {
          continue;
        }

        const gf::TileData tile_data = std::get<gf::TileData>(object_data.feature);
        const gf::TilesetData* tileset_data = map_data->data.tileset_from_gid(tile_data.gid);
        assert(tileset_data != nullptr);
        const uint32_t gid = tile_data.gid - tileset_data->first_gid;
        assert(gid < 3);

        const gf::RectF bounds = gf::RectF::from_bottom_left_size(object_data.location + object_layer.layer.offset, tileset_data->tile_size);

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

    // clang-format off
    m_supplies.erase(std::remove_if(m_supplies.begin(), m_supplies.end(), [](const Supply& supply) {
      return supply.current_quantity <= 0;
    }), m_supplies.end());
    // clang-format on

    if (is_mining) {
      gf::ShapeGroupData shape_group_data;

      for (const auto& supply : m_supplies) {
        if (supply.current_quantity == supply.initial_quantity) {
          continue;
        }

        auto bounds = supply.bounds;
        bounds.extent.h = 10.0f;
        auto color = to_color(supply.type);

        gf::ShapeData back_shape_data = gf::ShapeData::make_rectangle(bounds);
        back_shape_data.color = gf::darker(color) * gf::opaque(0.3f);
        back_shape_data.outline_color = gf::darker(color);
        back_shape_data.outline_thickness = 2.0f;
        shape_group_data.shapes.emplace_back(std::move(back_shape_data));

        const float percent = static_cast<float>(supply.current_quantity) / static_cast<float>(supply.initial_quantity);
        bounds.extent.w *= percent;

        gf::ShapeData front_shape_data = gf::ShapeData::make_rectangle(bounds);
        front_shape_data.color = color;
        shape_group_data.shapes.emplace_back(std::move(front_shape_data));
      }

      m_shapes.update(shape_group_data, m_hub->render_manager());
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

    if (geometry.size > 0) {
      gf::RenderObject object = {};
      object.priority = priority();
      object.geometry = geometry;
      recorder.record(object);
    }
  }

}
