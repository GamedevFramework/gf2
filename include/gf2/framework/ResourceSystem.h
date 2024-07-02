// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_RESOURCE_SYSTEM_H
#define GF_RESOURCE_SYSTEM_H

#include <gf2/core/FontFace.h>
#include <gf2/core/ResourceLoaders.h>
#include <gf2/core/ResourceManager.h>
#include <gf2/core/TiledMapData.h>

#include <gf2/audio/Music.h>
#include <gf2/audio/Sound.h>
#include <gf2/graphics/Texture.h>
#include <gf2/graphics/TiledMap.h>
#include <gf2/graphics/ConsoleFont.h>

#include <utility>

#include "FrameworkApi.h"

namespace gf {

  class GF_FRAMEWORK_API ResourceSystem : public ResourceManager {
  public:
    ResourceSystem(const std::filesystem::path& asset_directory);

    void add_search_directory(std::filesystem::path directory)
    {
      m_file_loader.add_search_directory(std::move(directory));
    }

    std::filesystem::path search(const std::filesystem::path& relative_path)
    {
      return m_file_loader.search(relative_path);
    }

  private:
    FileLoader m_file_loader;

    // from core
    ResourceRegistry<FontFace> m_font_registry;
    ResourceRegistry<TiledMapResource> m_tiled_map_resource_registry;
    // from graphics
    ResourceRegistry<ConsoleFont> m_console_font_registry;
    ResourceRegistry<Texture> m_texture_registry;
    ResourceRegistry<TiledMap> m_tiled_map_registry;
    // from audio
    ResourceRegistry<Sound> m_sound_registry;
    ResourceRegistry<Music> m_music_registry;
  };

}

#endif // GF_RESOURCE_SYSTEM_H
