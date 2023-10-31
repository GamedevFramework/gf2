#include "WorldScene.h"

#include "GameHub.h"

namespace home {

  WorldScene::WorldScene(GameHub* hub)
  : m_title(m_data.title, hub->render_manager(), hub->resource_manager())
  {
    set_clear_color(gf::Color(0xAEF6B8));
    set_world_center({ 0.0f, 0.0f });
    set_world_size({ 800.0f, 800.0f });
    m_title.transform().location = { 0.0f, 0.0f };
    m_title.transform().origin = { 0.5f, 0.5f };
    add_world_entity(&m_title);
  }

  gf::ResourceBundle WorldScene::bundle(GameHub* hub)
  {
    gf::ResourceBundle bundle([hub](gf::ResourceBundle* bundle, gf::ResourceManager* resources, gf::ResourceBundle::Action action) {
      bundle->handle<gf::Font>("Xolonium-Regular.ttf", { hub->font_manager(), hub->render_manager() }, resources, action);
    });
    return bundle;
  }

}
