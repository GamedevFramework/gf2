#include "WorldData.h"

#include "GameHub.h"

namespace home {

  WorldData::WorldData()
  {
  }

  gf::ResourceBundle WorldData::bundle(GameHub* hub)
  {
    gf::ResourceBundle bundle([hub, this](gf::ResourceBundle* bundle, gf::ResourceManager* resources, gf::ResourceAction action) {
      // fonts

      // for (const gf::TextResource& resource : {  }) {
      //   bundle->handle<gf::Font>(resource.font, { hub->font_manager(), hub->render_manager() }, resources, action);
      // }

      // textures

      // for (const gf::SpriteResource& resource : {  }) {
      //   bundle->handle<gf::Texture>(resource.texture, hub->render_manager(), resources, action);
      // }
    });

    return bundle;
  }

}
