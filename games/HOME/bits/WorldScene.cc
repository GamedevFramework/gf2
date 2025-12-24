#include "WorldScene.h"

#include "GameHub.h"

namespace home {

  namespace {

    gf::PhysicsWorldData world_data()
    {
      gf::PhysicsWorldData data;
      data.gravity = { 0.0f, 0.0f };
      return data;
    }

    gf::PhysicsDebugOptions debug_option()
    {
      gf::PhysicsDebugOptions options;
      options.features = gf::All;
      return options;
    }

  }

  WorldScene::WorldScene(GameHub* hub, const WorldResources& resources)
  : m_hub(hub)
  , m_action_group(compute_settings())
  , m_main_theme_music(hub->resource_manager()->get<gf::Music>(resources.main_theme_music.filename))
  , m_breath_low_o2_sound(hub->resource_manager()->get<gf::Sound>(resources.breath_low_o2_sound.filename))
  , m_victory_sound(hub->resource_manager()->get<gf::Sound>(resources.victory_sound.filename))
  , m_death_sound(hub->resource_manager()->get<gf::Sound>(resources.death_sound.filename))
  , m_physics_world(world_data())
  , m_physics_debug(debug_option(), &m_physics_world, hub->render_manager())
  , m_map_entity(hub, resources, &m_physics_world)
  , m_supply_entity(hub, resources)
  , m_hero_entity(hub, resources, &m_physics_world)
  , m_backpack_entity(hub, resources)
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

  gf::ActionGroupSettings WorldScene::compute_settings()
  {
    using namespace gf::literals;
    gf::ActionGroupSettings settings;

    settings.actions.emplace("debug"_id, gf::ActionSettings(gf::ActionType::Instantaneous).add_keycode_control(gf::Keycode::P));

    return settings;
  }

  void WorldScene::do_update(gf::Time time)
  {
    update_entities(time);
  }

  void WorldScene::do_process_event(const gf::Event& event)
  {
    if (event.type() == gf::EventType::MouseButtonPressed) {
      auto target = position_to_world_location(event.from<gf::EventType::MouseButtonPressed>().position);
      gf::Log::debug("target: {}, {}", target.x, target.y);
      m_hero_entity.set_target(target);
    }

    m_action_group.process_event(event);
  }

  void WorldScene::do_handle_actions()
  {
    using namespace gf::literals;

    if (m_action_group.active("debug"_id)) {
      m_debug = !m_debug;
      m_physics_debug.set_awake(m_debug);
    }

    m_action_group.reset();
  }

}
