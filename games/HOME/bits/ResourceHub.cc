#include "ResourceHub.h"

namespace home {

  ResourceHub::ResourceHub(const std::filesystem::path& asset_directory)
  {
    file_loader.add_search_directory(asset_directory);

    font_registry.add_loader(gf::loader_for<gf::FontFace>(file_loader));
    texture_registry.add_loader(gf::loader_for<gf::Texture>(file_loader));
    sound_registry.add_loader(gf::loader_for<gf::Sound>(file_loader));
    music_registry.add_loader(gf::loader_for<gf::Music>(file_loader));
    tiled_map_resource_registry.add_loader(gf::loader_for<gf::TiledMapResource>(file_loader));
    tiled_map_registry.add_loader(gf::loader_for<gf::TiledMap>(file_loader));

    manager.add_registry(&texture_registry);
    manager.add_registry(&font_registry);
    manager.add_registry(&sound_registry);
    manager.add_registry(&music_registry);
    manager.add_registry(&tiled_map_resource_registry);
    manager.add_registry(&tiled_map_registry);
  }

}
