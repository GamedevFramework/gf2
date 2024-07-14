#include "SplashData.h"

#include "GameHub.h"

namespace home {

  SplashData::SplashData()
  {
    using namespace gf::literals;

    // action_group

    action_group.actions.emplace("click"_id, gf::ActionSettings(gf::ActionType::Instantaneous).add_mouse_button_control(gf::AnyMouseButton));

    // title_sprite

    title_sprite.texture = "images/title.png";

    // title_text

    title_text.font = "Xolonium-Regular.ttf";
    title_text.data.content = "Harvest Oxygen in the Maldoran Ecosystem";
    title_text.data.character_size = 72.0f;
    title_text.data.color = gf::Color(0x44548E);

    // click_text

    click_text.font = "Xolonium-Regular.ttf";
    click_text.data.content = "Click anywhere to continue...";
    click_text.data.character_size = 36.0f;
    click_text.data.color = gf::Color(0x44548E);
  }

  gf::ResourceBundle SplashData::bundle(GameHub* hub)
  {
    gf::ResourceBundle bundle([hub, this](gf::ResourceBundle* bundle, gf::ResourceManager* resources, gf::ResourceAction action) {
      // fonts

      for (const gf::TextResource* resource : { &title_text, &click_text }) {
        bundle->handle<gf::FontFace>(resource->font, hub->font_manager(), resources, action);
      }

      // textures

      for (const gf::SpriteResource* resource : { &title_sprite }) {
        bundle->handle<gf::Texture>(resource->texture, hub->render_manager(), resources, action);
      }
    });

    return bundle;
  }

}
