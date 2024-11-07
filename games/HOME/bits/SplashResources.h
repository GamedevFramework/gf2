#ifndef HOME_SPLASH_DATA_H
#define HOME_SPLASH_DATA_H

#include <gf2/core/ActionSettings.h>
#include <gf2/core/ResourceBundle.h>
#include <gf2/core/SpriteData.h>
#include <gf2/core/TextData.h>

namespace home {
  class GameHub;

  struct SplashResources {
    SplashResources();

    gf::ResourceBundle bundle(GameHub* hub);

    gf::SpriteResource title_sprite;
    gf::TextResource title_text;
    gf::TextResource click_text;
  };

}

#endif // HOME_SPLASH_DATA_H
