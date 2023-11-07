#include "SplashScene.h"

#include "GameHub.h"

namespace home {

  SplashScene::SplashScene(GameHub* hub, const SplashData& data)
  : m_hub(hub)
  , m_action_group(data.action_group)
  , m_title_sprite(data.title_sprite, hub->render_manager(), hub->resource_manager())
  , m_title_text(data.title_text, hub->render_manager(), hub->resource_manager())
  , m_click_text(data.click_text, hub->render_manager(), hub->resource_manager())
  {
    set_clear_color(gf::Color(0xAEF6B8));
    set_world_center({ 0.0f, 0.0f });
    set_world_size({ 1200.0f, 1200.0f });

    m_title_sprite.set_location({ 0.0f, 0.0f });
    m_title_sprite.set_origin({ 0.5f, 0.5f });
    add_world_entity(&m_title_sprite);

    m_title_text.set_location({ 0.0f, 400.0f });
    m_title_text.set_origin({ 0.5f, 0.5f });
    m_title_text.text().set_characater_size(72.0f);
    add_world_entity(&m_title_text);

    m_click_text.set_location({ 0.0f, 500.0f });
    m_click_text.set_origin({ 0.5f, 0.5f });
    m_click_text.text().set_characater_size(36.0f);
    add_world_entity(&m_click_text);
  }

  void SplashScene::do_process_event(const gf::Event& event)
  {
    m_action_group.process_event(event);
  }

  void SplashScene::do_handle_actions()
  {
    using namespace gf::literals;

    if (m_action_group.active("click"_id)) {
      m_hub->replace_scene(m_hub->world_scene());
    }

    m_action_group.reset();
  }
}
