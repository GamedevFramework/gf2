// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_RESOURCE_SYSTEM_H
#define GF_RESOURCE_SYSTEM_H

#include <utility>

#include <gf2/core/FontFace.h>
#include <gf2/core/Image.h>
#include <gf2/core/ResourceLoaders.h>
#include <gf2/core/ResourceManager.h>
#include <gf2/core/SpriteSheet.h>
#include <gf2/core/TiledMap.h>

#include <gf2/audio/Music.h>
#include <gf2/audio/Sound.h>
#include <gf2/graphics/ConsoleFont.h>
#include <gf2/graphics/GpuTexture.h>
#include <gf2/graphics/RichMap.h>

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
    ResourceRegistry<Image> m_image_registry;
    ResourceRegistry<FontFace> m_font_registry;
    ResourceRegistry<SpriteSheet> m_sprite_sheet_registry;
    ResourceRegistry<TiledMap> m_tiled_map_registry;
    // from graphics
    ResourceRegistry<ConsoleFont> m_console_font_registry;
    ResourceRegistry<GpuTexture> m_texture_registry;
    ResourceRegistry<RichMap> m_rich_map_registry;
    // from audio
    ResourceRegistry<Sound> m_sound_registry;
    ResourceRegistry<Music> m_music_registry;
  };

}

#endif // GF_RESOURCE_SYSTEM_H
