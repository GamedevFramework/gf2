#include "GameHub.h"

namespace home {

  GameHub::GameHub(const std::filesystem::path& asset_directory)
  : gf::SingleSceneManager("H.O.M.E.", { 1600, 900 })
  , m_resource_hub(asset_directory)
  {
    auto world_bundle = m_world_data.bundle(this);
    world_bundle.load_from(resource_manager());

    m_world_scene = std::make_unique<WorldScene>(this, m_world_data);
    set_scene(world_scene());
  }

}
