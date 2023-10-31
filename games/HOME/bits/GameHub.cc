#include "GameHub.h"

namespace home {

  GameHub::GameHub(const std::filesystem::path& asset_directory)
  : gf::SingleSceneManager("H.O.M.E.", { 1600, 900 })
  , resource_hub(asset_directory)
  {
    auto world_bundle = WorldScene::bundle(this);
    world_bundle.load_from(resource_hub.manager);

    world_scene = std::make_unique<WorldScene>(this);

    set_scene(world_scene.get());
  }

}
