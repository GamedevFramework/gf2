#include "WorldScene.h"

#include "GameHub.h"

namespace home {
  using namespace gf::literals;

  WorldScene::WorldScene(GameHub* hub, const WorldData& data)
  : main_theme_music(hub->resource_manager()->get<gf::Music>(data.main_theme_music.filename))
  , mining_sound(hub->resource_manager()->get<gf::Sound>(data.mining_sound.filename))
  , o2_filling_sound(hub->resource_manager()->get<gf::Sound>(data.o2_filling_sound.filename))
  , breath_low_o2_sound(hub->resource_manager()->get<gf::Sound>(data.breath_low_o2_sound.filename))
  , victory_sound(hub->resource_manager()->get<gf::Sound>(data.victory_sound.filename))
  , death_sound(hub->resource_manager()->get<gf::Sound>(data.death_sound.filename))
  , m_hero_entity(hub, data)
  {
    set_clear_color(gf::Color(0xAEF6B8));
    set_world_center({ 0.0f, 0.0f });
    set_world_size({ 800.0f, 800.0f });

    add_world_entity(&m_hero_entity);

    m_hero_entity.update_location.connect([this](gf::Vec2F location) { set_world_center(location); });

  }

  void WorldScene::do_process_event(const gf::Event& event)
  {
    if (event.type == gf::EventType::MouseButtonPressed) {
      m_hero_entity.set_target(position_to_world_location(event.mouse_button.position));
    }
  }

}
