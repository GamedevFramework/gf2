#include "WorldData.h"

#include <gf2/core/Property.h>

#include <gf2/audio/Music.h>
#include <gf2/audio/Sound.h>
#include <gf2/graphics/TiledMap.h>

#include "GameHub.h"
#include "SupplyType.h"

namespace home {

  WorldData::WorldData()
  : map("map/Map.tmx")
  {
    using namespace gf::literals;

    action_group.actions.emplace("debug"_id, gf::ActionData(gf::ActionType::Instantaneous).add_scancode_control(gf::Scancode::F12));

    main_theme_music.filename = "sounds/main_theme.ogg";
    main_theme_music.data.type = gf::AudioSourceType::Music;
    main_theme_music.data.loop = true;

    jet_engine_sound.filename = "sounds/jet_engine.ogg";

    mining_sound.filename = "sounds/mining.ogg";
    mining_sound.data.loop = true;
    oxygen_sound.filename = "sounds/o2_filling.ogg";
    oxygen_sound.data.loop = true;

    breath_low_o2_sound.filename = "sounds/breath_low_o2.ogg";
    victory_sound.filename = "sounds/win.ogg";
    death_sound.filename = "sounds/death.ogg";

    hero_animations.textures = { "images/player_animations.png", "images/player_death.png" };

    gf::AnimationData animation;
    animation.properties = gf::AnimationProperties::Loop;
    animation.color = gf::White;
    int animation_index = 0;

    const gf::Id animation_ids[] = {
      "move_west"_id,
      "move_north_west"_id,
      "move_north"_id,
      "move_north_east"_id,
      "move_east"_id,
      "move_south_east"_id,
      "move_south_west"_id,
      "pause_west"_id,
      "pause_north_west"_id,
      "pause_north"_id,
      "pause_north_east"_id,
      "pause_east"_id,
      "pause_south_east"_id,
      "pause_south_west"_id,
      "south"_id,
      "harvest_north_west"_id,
      "harvest_north_east"_id,
      "harvest_south_east"_id,
      "harvest_south_west"_id,
    };

    for (auto id : animation_ids) {
      animation.frames.clear();
      animation.add_tileset(0, gf::vec(22, 19), gf::seconds(1.0f / 30.f), 22, animation_index * 22);
      hero_animations.data.animations.emplace(id, animation);
      ++animation_index;
    }

    animation.properties = gf::None;
    animation.frames.clear();
    animation.add_tileset(1, gf::vec(22, 4), gf::seconds(1.0f / 30.f), 83, 0);
    animation.add_tileset(1, gf::vec(22, 4), gf::seconds(60.0f * 60.0f * 24.0f * 10000.0f), 1, 83);
    hero_animations.data.animations.emplace("death"_id, animation);

    crosshair.texture = "crosshair.png";

    constexpr float ResourceUnitX = 1.0f / 7.0f;
    constexpr float ResourceUnitY = 1.0f / 5.0f;

    energy_sprite.texture = "map/ResourceSet.png";
    energy_sprite.data.texture_region = gf::RectF::from_position_size({ 0 * ResourceUnitX, 0.0f }, { ResourceUnitX, ResourceUnitY });
    metal_sprite.texture = "map/ResourceSet.png";
    metal_sprite.data.texture_region = gf::RectF::from_position_size({ 1 * ResourceUnitX, 0.0f }, { ResourceUnitX, ResourceUnitY });
    oxygen_sprite.texture = "map/ResourceSet.png";
    oxygen_sprite.data.texture_region = gf::RectF::from_position_size({ 2 * ResourceUnitX, 0.0f }, { ResourceUnitX, ResourceUnitY });

    backpack_icon.texture = "images/inventory_icon.png";
    oxygen_icon.texture = "images/oxygen_icon.png";
    oxygen_icon.data.color = gf::darker(to_color(SupplyType::Oxygen));
  }

  gf::ResourceBundle WorldData::bundle(GameHub* hub)
  {
    gf::ResourceBundle bundle([hub, this](gf::ResourceBundle* bundle, gf::ResourceManager* resources, gf::ResourceAction action) {
      // map

      bundle->handle<gf::TiledMap>(map, { hub->render_manager(), resources }, resources, action);

      // fonts

      // for (const gf::TextResource& resource : {  }) {
      //   bundle->handle<gf::Font>(resource.font, { hub->font_manager(), hub->render_manager() }, resources, action);
      // }

      // textures

      for (const gf::SpriteResource& resource : { crosshair, energy_sprite, metal_sprite, oxygen_sprite, backpack_icon, oxygen_icon }) {
        bundle->handle<gf::Texture>(resource.texture, hub->render_manager(), resources, action);
      }

      // audio sources

      for (const gf::AudioSourceResource& resource : { main_theme_music, jet_engine_sound, mining_sound, oxygen_sound, breath_low_o2_sound, victory_sound, death_sound }) {
        switch (resource.data.type) {
          case gf::AudioSourceType::Music:
            bundle->handle<gf::Music>(resource.filename, hub->audio_manager(), resources, action);
            break;
          case gf::AudioSourceType::Sound:
            bundle->handle<gf::Sound>(resource.filename, hub->audio_manager(), resources, action);
            break;
        }
      }

      // animation groups

      for (const gf::AnimationGroupResource& resource : { hero_animations }) {
        for (const auto& texture : resource.textures) {
          bundle->handle<gf::Texture>(texture, hub->render_manager(), resources, action);
        }
      }
    });

    return bundle;
  }

}
