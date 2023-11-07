#include "SplashData.h"

#include "GameHub.h"

namespace home {

  SplashData::SplashData()
  {
    using namespace gf::literals;

    // action_group

    gf::ControlData click_control = {};
    click_control.type = gf::ControlType::MouseButton;
    click_control.mouse_button.button = gf::AnyMouseButton;

    gf::ActionData click_action;
    click_action.type = gf::ActionType::Instantaneous;
    click_action.controls.push_back(click_control);

    action_group.actions.emplace("click"_id, click_action);

    // title_sprite

    title_sprite.texture = "images/title.png";

    // title_text

    title_text.font = "Xolonium-Regular.ttf";
    title_text.data.content = "Harvest Oxygen in the Maldoran Ecosystem";
    title_text.data.color = gf::Color(0x44548E);

    // click_text

    click_text.font = "Xolonium-Regular.ttf";
    click_text.data.content = "Click anywhere to continue...";
    click_text.data.color = gf::Color(0x44548E);
  }

  gf::ResourceBundle SplashData::bundle(GameHub* hub)
  {
    gf::ResourceBundle bundle([hub, this](gf::ResourceBundle* bundle, gf::ResourceManager* resources, gf::ResourceAction action) {
      // fonts

      for (const gf::TextResource& resource : { title_text, click_text }) {
        bundle->handle<gf::Font>(resource.font, { hub->font_manager(), hub->render_manager() }, resources, action);
      }

      // textures

      for (const gf::SpriteResource& resource : { title_sprite }) {
        bundle->handle<gf::Texture>(resource.texture, hub->render_manager(), resources, action);
      }
    });

    return bundle;
  }

}
