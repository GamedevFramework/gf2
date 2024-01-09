#include "BackpackEntity.h"

#include <gf2/core/Positioning.h>

#include <gf2/graphics/RenderRecorder.h>

#include "GameHub.h"
#include "SupplyEntity.h"

namespace home {

  namespace {

    constexpr int LimitBackpack = 4'000'000;

    constexpr int MaxOxygen = 100'000;
    constexpr float OxygenLoss = 1000.0f;

    constexpr int UnloadSpeed = 250'000;
  }

  BackpackEntity::BackpackEntity(GameHub* hub, const WorldData& data)
  : m_hub(hub)
  , m_oxygen_quantity(MaxOxygen)
  , m_backpack_sprite(data.backpack_icon, hub->render_manager(), hub->resource_manager())
  , m_oxygen_sprite(data.oxygen_icon, hub->render_manager(), hub->resource_manager())
  {

    const auto* map_data = hub->resource_manager()->get<gf::TiledMapResource>(data.map);

    for (const auto& object_layer : map_data->data.object_layers) {
      if (object_layer.layer.name != "SpaceShip") {
        continue;
      }

      assert(object_layer.objects.size() == 1);
      const auto& object_data = object_layer.objects.front();

      assert(object_data.type == gf::ObjectType::Tile);
      const gf::TileData tile_data = std::get<gf::TileData>(object_data.feature);

      const gf::TilesetData* tileset_data = map_data->data.tileset_from_gid(tile_data.gid);
      assert(tileset_data != nullptr);
      const gf::RectF bounds = gf::RectF::from_bottom_left_size(object_data.location + object_layer.layer.offset, tileset_data->tile_size);

      m_ship_bounds = bounds;
      gf::Log::info("ship bounds: ({}, {}), ({}, {})", m_ship_bounds.offset.x, m_ship_bounds.offset.y, m_ship_bounds.extent.w, m_ship_bounds.extent.h);
    }
  }

  void BackpackEntity::set_hero_location(gf::Vec2F location)
  {
    m_hero_location = location;
  }

  void BackpackEntity::update(gf::Time time)
  {
    if (m_oxygen_quantity > 0) {
      m_oxygen_quantity -= static_cast<int32_t>(time.as_seconds() * OxygenLoss);
    } else {
      // TODO: die!
    }

    if (!m_supplies.empty() && m_ship_bounds.distance_from(m_hero_location, gf::euclidean_distance) < 50.0f) {
      auto& supply = m_supplies.back();

      int quantity = static_cast<int32_t>(time.as_seconds() * UnloadSpeed);

      if (quantity > supply.quantity) {
        quantity = supply.quantity;
      }

      unload.emit(supply.type, quantity);

      supply.quantity -= quantity;

      if (supply.quantity == 0) {
        m_supplies.pop_back();
      }
    }

    gf::Positioning positioning(m_hub->render_manager()->surface_size());

    gf::ShapeGroupData shape_group_data;

    gf::RectF backpack_bounds = gf::RectF::from_position_size(positioning.relative_point({ 0.95f, 0.18f }), positioning.relative_size({ 0.01f, 0.72f }));

    gf::ShapeData back_shape_data = gf::ShapeData::make_rectangle(backpack_bounds);
    back_shape_data.color = gf::Black * gf::opaque(0.3f);
    back_shape_data.outline_color = gf::Black;
    back_shape_data.outline_thickness = positioning.relative_thickness(0.001f);
    shape_group_data.shapes.emplace_back(std::move(back_shape_data));

    auto supply_position = backpack_bounds.position_at(gf::Orientation::SouthWest);

    for (auto& supply : m_supplies) {
      float supply_percent = static_cast<float>(supply.quantity) / static_cast<float>(LimitBackpack);
      float supply_height = backpack_bounds.extent.h * supply_percent;
      supply_position.y -= supply_height;

      auto color = to_color(supply.type);

      gf::ShapeData supply_shape_data = gf::ShapeData::make_rectangle(gf::RectF::from_position_size(supply_position, { backpack_bounds.extent.w, supply_height }));
      supply_shape_data.color = color;
      supply_shape_data.outline_color = gf::darker(color);
      supply_shape_data.outline_thickness = positioning.relative_thickness(0.001f);
      shape_group_data.shapes.emplace_back(std::move(supply_shape_data));
    }

    gf::RectF oxygen_bounds = gf::RectF::from_position_size(positioning.relative_point({ 0.04f, 0.18f }), positioning.relative_size({ 0.01f, 0.72f }));
    gf::Color oxygen_color = to_color(SupplyType::Oxygen);

    auto oxygen_percent = static_cast<float>(m_oxygen_quantity) / static_cast<float>(MaxOxygen);

    gf::ShapeData oxygen_back_shape_data = gf::ShapeData::make_rectangle(oxygen_bounds);
    oxygen_back_shape_data.color = gf::darker(oxygen_color) * gf::opaque(0.3f);

    if (oxygen_percent < 0.2f) {
      oxygen_back_shape_data.outline_color = gf::Red;
    } else {
      oxygen_back_shape_data.outline_color = gf::darker(oxygen_color);
    }

    oxygen_back_shape_data.outline_thickness = positioning.relative_thickness(0.001f);
    shape_group_data.shapes.emplace_back(std::move(oxygen_back_shape_data));

    auto difference = oxygen_bounds.extent.h * (1.0f - oxygen_percent);
    oxygen_bounds.offset.y += difference;
    oxygen_bounds.extent.h -= difference;

    gf::ShapeData oxygen_front_shape_data = gf::ShapeData::make_rectangle(oxygen_bounds);
    oxygen_front_shape_data.color = oxygen_color;
    shape_group_data.shapes.emplace_back(std::move(oxygen_front_shape_data));

    m_shapes.update(shape_group_data, m_hub->render_manager());
  }

  void BackpackEntity::render(gf::RenderRecorder& recorder)
  {
    gf::Positioning positioning(m_hub->render_manager()->surface_size());

    auto icon_size = positioning.relative_size({ 0.04f, 0.0f });

    m_oxygen_sprite.set_location(positioning.relative_point({ 0.045f, 0.1f }));
    m_oxygen_sprite.set_origin({ 0.5f, 0.0f });
    m_oxygen_sprite.set_scale(icon_size.w / static_cast<float>(m_oxygen_sprite.sprite().texture()->size().w));
    m_oxygen_sprite.render(recorder);

    m_backpack_sprite.set_location(positioning.relative_point({ 0.955f, 0.1f }));
    m_backpack_sprite.set_origin({ 0.5f, 0.0f });
    m_backpack_sprite.set_scale(icon_size.w / static_cast<float>(m_backpack_sprite.sprite().texture()->size().w));
    m_backpack_sprite.render(recorder);

    auto geometry = m_shapes.geometry();

    if (geometry.size > 0) {
      gf::RenderObject object = {};
      object.priority = priority();
      object.geometry = geometry;
      recorder.record(object);
    }
  }

  void BackpackEntity::put_in_backpack(SupplyType type, int32_t quantity)
  {
    if (type == SupplyType::Oxygen) {
      m_oxygen_quantity += quantity;

      if (m_oxygen_quantity > MaxOxygen) {
        m_oxygen_quantity = MaxOxygen;
      }
    } else {
      if (m_total_quantity + quantity > LimitBackpack) {
        quantity = LimitBackpack - m_total_quantity;
      }

      if (m_supplies.empty() || m_supplies.back().type != type) {
        m_supplies.push_back({ type, quantity });
      } else {
        m_supplies.back().quantity += quantity;
      }

      m_total_quantity += quantity;
    }
  }

}