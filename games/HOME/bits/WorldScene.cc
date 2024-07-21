#include "WorldScene.h"

#include <gf2/physics/PhysicsArbiter.h>
#include <gf2/physics/PhysicsCollisionHandler.h>

#include "GameHub.h"

namespace home {

  using namespace gf::literals;

  WorldScene::WorldScene(GameHub* hub, const WorldData& data)
  : m_hub(hub)
  , m_action_group(data.action_group)
  , m_main_theme_music(hub->resource_manager()->get<gf::Music>(data.main_theme_music.filename))
  , m_breath_low_o2_sound(hub->resource_manager()->get<gf::Sound>(data.breath_low_o2_sound.filename))
  , m_victory_sound(hub->resource_manager()->get<gf::Sound>(data.victory_sound.filename))
  , m_death_sound(hub->resource_manager()->get<gf::Sound>(data.death_sound.filename))
  , m_physics_debug(&m_physics_world, hub->render_manager())
  , m_map_entity(hub, data, &m_physics_world)
  , m_supply_entity(hub, data)
  , m_hero_entity(hub, data, &m_physics_world)
  , m_backpack_entity(hub, data)
  {
    set_clear_color(gf::Color(0xAEF6B8));
    set_world_center({ 0.0f, 0.0f });
    set_world_size({ 800.0f, 800.0f });

    add_model(&m_physics_world);

    add_world_entity(&m_map_entity);
    add_world_entity(&m_supply_entity);
    add_world_entity(&m_hero_entity);

    add_world_entity(&m_physics_debug);

    add_hud_entity(&m_backpack_entity);

    // NOLINTNEXTLINE(bugprone-unused-return-value)
    m_hero_entity.update_location.connect([this](gf::Vec2F location) {
      set_world_center(location);
      m_map_entity.set_hero_location(location);
      m_supply_entity.set_hero_location(location);
      m_backpack_entity.set_hero_location(location);
    });

    // NOLINTNEXTLINE(bugprone-unused-return-value)
    m_supply_entity.harvest.connect([this](SupplyType type, int32_t quantity) {
      m_hero_entity.set_mining();
      m_backpack_entity.put_in_backpack(type, quantity);
    });
  }

  void WorldScene::do_update(gf::Time time)
  {
    update_entities(time);
  }

  void WorldScene::do_process_event(const gf::Event& event)
  {
    if (event.type == gf::EventType::MouseButtonPressed) {
      auto target = position_to_world_location(event.mouse_button.position);
      gf::Log::debug("target: {}, {}", target.x, target.y);
      m_hero_entity.set_target(target);
    }

    m_action_group.process_event(event);
  }

  void WorldScene::do_handle_actions()
  {
    if (m_action_group.active("debug"_id)) {
      m_debug = !m_debug;
      m_physics_debug.set_awake(m_debug);
    }

    m_action_group.reset();
  }

}
