#ifndef HOME_WORLD_DATA_H
#define HOME_WORLD_DATA_H

#include <filesystem>

#include <gf2/core/ActionData.h>
#include <gf2/core/AnimationData.h>
#include <gf2/core/AudioSourceData.h>
#include <gf2/core/ResourceBundle.h>
#include <gf2/core/SpriteData.h>
#include <gf2/core/TextData.h>

namespace home {
  class GameHub;

  struct WorldData {
    WorldData();

    gf::ResourceBundle bundle(GameHub* hub);

    gf::ActionGroupData action_group;

    std::filesystem::path map;

    gf::AudioSourceResource main_theme_music;

    gf::AudioSourceResource jet_engine_sound;
    gf::AudioSourceResource mining_sound;
    gf::AudioSourceResource o2_filling_sound;
    gf::AudioSourceResource breath_low_o2_sound;
    gf::AudioSourceResource victory_sound;
    gf::AudioSourceResource death_sound;

    gf::AnimationGroupResource hero_animations;

    gf::SpriteResource crosshair;
  };

}

#endif // HOME_WORLD_DATA_H
