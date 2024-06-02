#ifndef HOME_SPLASH_SCENE_H
#define HOME_SPLASH_SCENE_H

#include <gf2/core/ResourceBundle.h>

#include <gf2/graphics/ActionGroup.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SpriteEntity.h>
#include <gf2/graphics/TextEntity.h>

#include "SplashData.h"

namespace home {
  class GameHub;

  class SplashScene : public gf::Scene {
  public:
    SplashScene(GameHub* hub, const SplashData& data);

  private:
    void do_update(gf::Time time) override;
    void do_process_event(const gf::Event& event) override;
    void do_handle_actions() override;

    GameHub* m_hub = nullptr;

    bool m_world_loaded = false;

    gf::FontAtlas m_atlas;
    gf::ActionGroup m_action_group;
    gf::SpriteEntity m_title_sprite;
    gf::TextEntity m_title_text;
    gf::TextEntity m_click_text;
  };

}

#endif // HOME_SPLASH_SCENE_H
