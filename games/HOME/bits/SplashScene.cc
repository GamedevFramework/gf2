#include "SplashScene.h"

#include "GameHub.h"

namespace home {

  SplashScene::SplashScene(GameHub* hub, const SplashData& data)
  : m_title_text(data.title_text, hub->render_manager(), hub->resource_manager())
  , m_title_sprite(data.title_sprite, hub->render_manager(), hub->resource_manager())
  {
    set_clear_color(gf::srgb_to_linear(gf::Color(0xAEF6B8)));
    set_world_center({ 0.0f, 0.0f });
    set_world_size({ 1200.0f, 1200.0f });

    m_title_sprite.transform().location = { 0.0f, 0.0f };
    m_title_sprite.transform().origin = { 0.5f, 0.5f };
    add_world_entity(&m_title_sprite);

    m_title_text.transform().location = { 0.0f, 400.0f };
    m_title_text.transform().origin = { 0.5f, 0.5f };
    m_title_text.text().set_characater_size(72.0f);
    add_world_entity(&m_title_text);
  }

}
