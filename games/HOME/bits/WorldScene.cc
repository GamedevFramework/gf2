#include "WorldScene.h"

#include "GameHub.h"

namespace home {
  using namespace gf::literals;

  WorldScene::WorldScene(GameHub* hub, const WorldData& data)
  : main_theme_music(hub->resource_manager()->get<gf::Music>(data.main_theme_music.filename))
  , jet_engine_sound(hub->resource_manager()->get<gf::Sound>(data.jet_engine_sound.filename))
  , mining_sound(hub->resource_manager()->get<gf::Sound>(data.mining_sound.filename))
  , o2_filling_sound(hub->resource_manager()->get<gf::Sound>(data.o2_filling_sound.filename))
  , breath_low_o2_sound(hub->resource_manager()->get<gf::Sound>(data.breath_low_o2_sound.filename))
  , victory_sound(hub->resource_manager()->get<gf::Sound>(data.victory_sound.filename))
  , death_sound(hub->resource_manager()->get<gf::Sound>(data.death_sound.filename))
  , m_hero_animations(data.hero_animations, hub->render_manager(), hub->resource_manager())
  {
    set_clear_color(gf::Color(0xAEF6B8));
    set_world_center({ 0.0f, 0.0f });
    set_world_size({ 800.0f, 800.0f });

    m_hero_animations.select("death"_id);
    m_hero_animations.set_origin({ 0.5f, 0.5f });
    m_hero_animations.set_location({ 0.0f, 0.0f });
    add_world_entity(&m_hero_animations);
  }

}
