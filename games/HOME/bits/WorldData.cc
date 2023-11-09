#include "WorldData.h"

#include <gf2/audio/Sound.h>
#include <gf2/audio/Music.h>

#include "GameHub.h"

namespace home {

  WorldData::WorldData()
  {
    main_theme_music.filename = "sounds/main_theme.ogg";
    main_theme_music.data.loop = true;

    jet_engine_sound.filename = "sounds/jet_engine.ogg";
    mining_sound.filename = "sounds/mining.ogg";
    o2_filling_sound.filename = "sounds/o2_filling.ogg";
    breath_low_o2_sound.filename = "sounds/breath_low_o2.ogg";
    victory_sound.filename = "sounds/win.ogg";
    death_sound.filename = "sounds/death.ogg";
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

      // music

      for (const gf::AudioSourceResource& resource : { main_theme_music }) {
        bundle->handle<gf::Music>(resource.filename, hub->audio_manager(), resources, action);
      }

      // sounds

      for (const gf::AudioSourceResource& resource : { jet_engine_sound, mining_sound, o2_filling_sound, breath_low_o2_sound, victory_sound, death_sound }) {
        bundle->handle<gf::Sound>(resource.filename, hub->audio_manager(), resources, action);
      }
    });

    return bundle;
  }

}
