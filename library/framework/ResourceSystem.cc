// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/framework/ResourceSystem.h>

namespace gf {

  ResourceSystem::ResourceSystem(const std::filesystem::path& asset_directory)
  {
    m_file_loader.add_search_directory(asset_directory);

    m_font_registry.add_loader(loader_for<FontFace>(m_file_loader));
    m_tiled_map_resource_registry.add_loader(loader_for<TiledMapResource>(m_file_loader));
    m_console_font_registry.add_loader(loader_for<ConsoleFont>(m_file_loader));
    m_texture_registry.add_loader(loader_for<Texture>(m_file_loader));
    m_tiled_map_registry.add_loader(loader_for<TiledMap>(m_file_loader));
    m_sound_registry.add_loader(loader_for<Sound>(m_file_loader));
    m_music_registry.add_loader(loader_for<Music>(m_file_loader));

    add_registry(&m_font_registry);
    add_registry(&m_tiled_map_resource_registry);
    add_registry(&m_console_font_registry);
    add_registry(&m_texture_registry);
    add_registry(&m_tiled_map_registry);
    add_registry(&m_sound_registry);
    add_registry(&m_music_registry);
  }


}
