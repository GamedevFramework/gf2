#include "WorldScene.h"

#include "GameHub.h"

namespace home {

  namespace {
    gf::TextData title_text() {
      gf::TextData data;
      data.content = "H.O.M.E.";
      data.character_size = 60;
      data.color = gf::Color(0x44548E);
      return data;
    }
  }

  WorldScene::WorldScene(GameHub* hub)
  : m_title(hub->resource_hub.manager.get<gf::Font>("Xolonium-Regular.ttf")->atlas(), title_text(), hub->render_manager())
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
    gf::ResourceBundle bundle([hub](gf::ResourceBundle& bundle, gf::ResourceManager& resources, gf::ResourceBundle::Action action) {
      bundle.handle<gf::Font>("Xolonium-Regular.ttf", { &hub->font_manager, hub->render_manager() }, resources, action);
    });
    return bundle;
  }

}
