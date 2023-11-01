#ifndef HOME_WORLD_DATA_H
#define HOME_WORLD_DATA_H

#include <gf2/core/ResourceBundle.h>
#include <gf2/core/SpriteData.h>
#include <gf2/core/TextData.h>

namespace home {
  class GameHub;

  struct WorldData {
    WorldData();

    gf::ResourceBundle bundle(GameHub* hub);

    gf::TextResource title_text;
    gf::SpriteResource title_sprite;
  };

}

#endif // HOME_WORLD_DATA_H
