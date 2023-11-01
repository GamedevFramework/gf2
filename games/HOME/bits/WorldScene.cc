#include "WorldScene.h"

#include "GameHub.h"

namespace home {

  WorldScene::WorldScene(GameHub* hub, const WorldData& data)
  : m_title(data.title_text, hub->render_manager(), hub->resource_manager())
  {
    set_clear_color(gf::Color(0xAEF6B8));
    set_world_center({ 0.0f, 0.0f });
    set_world_size({ 800.0f, 800.0f });
    m_title.transform().location = { 0.0f, 0.0f };
    m_title.transform().origin = { 0.5f, 0.5f };
    add_world_entity(&m_title);
  }

}
