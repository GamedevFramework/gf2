#include "WorldScene.h"

#include <gf2/physics/PhysicsArbiter.h>
#include <gf2/physics/PhysicsCollisionHandler.h>

#include "GameHub.h"

namespace home {
  namespace {

#if 0
    class PhysicsCollisionHandler : public gf::PhysicsCollisionHandler {

      bool begin([[maybe_unused]] gf::PhysicsArbiter arbiter, [[maybe_unused]] gf::PhysicsWorld world) override
      {
        gf::Log::info("Collision! {}", arbiter.depth(0));
        return true;
      }

      bool pre_solve([[maybe_unused]] gf::PhysicsArbiter arbiter, [[maybe_unused]] gf::PhysicsWorld world) override
      {
        gf::Log::info("PreSolve");
        return true;
      }

      void post_solve([[maybe_unused]] gf::PhysicsArbiter arbiter, [[maybe_unused]] gf::PhysicsWorld world) override
      {
        gf::Log::info("PostSolve");
      }


      void separate([[maybe_unused]] gf::PhysicsArbiter arbiter, [[maybe_unused]] gf::PhysicsWorld world) override
      {
        gf::Log::info("Separate! {}", arbiter.count());
      }
    };

    PhysicsCollisionHandler handler;
#endif
  }

  using namespace gf::literals;

  WorldScene::WorldScene(GameHub* hub, const WorldData& data)
  : m_hub(hub)
  , m_action_group(data.action_group)
  , m_main_theme_music(hub->resource_manager()->get<gf::Music>(data.main_theme_music.filename))
  , m_mining_sound(hub->resource_manager()->get<gf::Sound>(data.mining_sound.filename))
  , m_o2_filling_sound(hub->resource_manager()->get<gf::Sound>(data.o2_filling_sound.filename))
  , m_breath_low_o2_sound(hub->resource_manager()->get<gf::Sound>(data.breath_low_o2_sound.filename))
  , m_victory_sound(hub->resource_manager()->get<gf::Sound>(data.victory_sound.filename))
  , m_death_sound(hub->resource_manager()->get<gf::Sound>(data.death_sound.filename))
  , m_physics_debug(hub->render_manager())
  , m_map_entity(hub, data, &m_physics_world)
  , m_hero_entity(hub, data, &m_physics_world)
  {
    set_clear_color(gf::Color(0xAEF6B8));
    set_world_center({ 0.0f, 0.0f });
    set_world_size({ 800.0f, 800.0f });

    // m_physics_world.add_default_collision_handler(&handler);

    add_model(&m_physics_world);

    add_world_entity(&m_map_entity);
    add_world_entity(&m_hero_entity);

    add_world_entity(m_physics_debug.entity());

    m_hero_entity.update_location.connect([this](gf::Vec2F location) { set_world_center(location); });
    m_hero_entity.update_location.connect([this](gf::Vec2F location) { m_map_entity.set_hero_location(location); });
  }

  void WorldScene::do_update(gf::Time time)
  {
    update_entities(time);

    m_physics_debug.start_debug();

    if (m_debug) {
      m_physics_world.debug_draw(&m_physics_debug);
    }

    m_physics_debug.stop_debug();
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
    }

    m_action_group.reset();
  }

}
