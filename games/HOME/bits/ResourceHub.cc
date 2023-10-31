#include "ResourceHub.h"

namespace home {

  ResourceHub::ResourceHub(const std::filesystem::path& asset_directory)
  {
    file_loader.add_search_directory(asset_directory);

    font_registry.add_loader(gf::loader_for<gf::Font>(file_loader));
    texture_registry.add_loader(gf::loader_for<gf::Texture>(file_loader));

    manager.add_registry(&texture_registry);
    manager.add_registry(&font_registry);
  }

}
