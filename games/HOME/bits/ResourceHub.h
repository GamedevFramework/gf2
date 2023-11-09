#ifndef HOME_RESOURCE_HUB_H
#define HOME_RESOURCE_HUB_H

#include <gf2/core/ResourceLoaders.h>
#include <gf2/core/ResourceManager.h>

#include "gf2/audio/Music.h"
#include "gf2/audio/Sound.h"
#include <gf2/graphics/Font.h>
#include <gf2/graphics/Texture.h>

namespace home {

  struct ResourceHub {
    ResourceHub(const std::filesystem::path& asset_directory);

    gf::FileLoader file_loader;

    gf::ResourceRegistry<gf::Font> font_registry;
    gf::ResourceRegistry<gf::Texture> texture_registry;
    gf::ResourceRegistry<gf::Sound> sound_registry;
    gf::ResourceRegistry<gf::Music> music_registry;

    gf::ResourceManager manager;
  };

}

#endif // HOME_RESOURCE_HUB_H
