#include "WorldScene.h"

#include "GameHub.h"

namespace home {
  using namespace gf::literals;

  WorldScene::WorldScene(GameHub* hub, const WorldData& data)
  : m_hub(hub)
  , m_main_theme_music(hub->resource_manager()->get<gf::Music>(data.main_theme_music.filename))
  , m_mining_sound(hub->resource_manager()->get<gf::Sound>(data.mining_sound.filename))
  , m_o2_filling_sound(hub->resource_manager()->get<gf::Sound>(data.o2_filling_sound.filename))
  , m_breath_low_o2_sound(hub->resource_manager()->get<gf::Sound>(data.breath_low_o2_sound.filename))
  , m_victory_sound(hub->resource_manager()->get<gf::Sound>(data.victory_sound.filename))
  , m_death_sound(hub->resource_manager()->get<gf::Sound>(data.death_sound.filename))
  , m_map_entity(hub, data)
  , m_hero_entity(hub, data)
  {
    set_clear_color(gf::Color(0xAEF6B8));
    set_world_center({ 0.0f, 0.0f });
    set_world_size({ 800.0f, 800.0f });

    add_model(m_hub->physics_world());

    add_world_entity(&m_map_entity);
    add_world_entity(&m_hero_entity);

    m_hero_entity.update_location.connect([this](gf::Vec2F location) { set_world_center(location); });
  }

  void WorldScene::do_process_event(const gf::Event& event)
  {
    if (event.type == gf::EventType::MouseButtonPressed) {
      auto target = position_to_world_location(event.mouse_button.position);
      gf::Log::debug("target: {}, {}", target.x, target.y);
      m_hero_entity.set_target(target);
    }
  }

}
