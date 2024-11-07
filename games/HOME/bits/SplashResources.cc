#include "SplashResources.h"

#include <gf2/framework/BundleBuilder.h>

#include "GameHub.h"

namespace home {

  SplashResources::SplashResources()
  {
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

  gf::ResourceBundle SplashResources::bundle(GameHub* hub) const
  {
    gf::BundleBuilder builder(hub);

    builder.add_in_bundle(title_sprite);
    builder.add_in_bundle(title_text);
    builder.add_in_bundle(click_text);

    return builder.make_bundle();
  }

}
