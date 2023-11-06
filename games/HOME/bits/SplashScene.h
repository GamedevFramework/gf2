#ifndef HOME_SPLASH_SCENE_H
#define HOME_SPLASH_SCENE_H

#include <gf2/core/ResourceBundle.h>

#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SpriteEntity.h>
#include <gf2/graphics/TextEntity.h>

#include "SplashData.h"

namespace home {
  class GameHub;

  class SplashScene : public gf::StandardScene {
  public:
    SplashScene(GameHub* hub, const SplashData& data);

  private:
    gf::TextEntity m_title_text;
    gf::SpriteEntity m_title_sprite;
  };

}

#endif // HOME_SPLASH_SCENE_H
