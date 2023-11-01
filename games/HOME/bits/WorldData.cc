#include "WorldData.h"

#include "GameHub.h"

#include "gf2/core/SpriteData.h"
#include "gf2/graphics/Texture.h"

namespace home {

  WorldData::WorldData()
  {
    // title
    title_text.font = "Xolonium-Regular.ttf";
    title_text.data.content = "H.O.M.E.";
    title_text.data.character_size = 60;
    title_text.data.color = gf::Color(0x44548E);

    // title image
    title_sprite.texture = "images/title.png";
  }

  gf::ResourceBundle WorldData::bundle(GameHub* hub)
  {
    gf::ResourceBundle bundle([hub, this](gf::ResourceBundle* bundle, gf::ResourceManager* resources, gf::ResourceAction action) {
      // fonts

      for (const gf::TextResource& resource : { title_text }) {
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
