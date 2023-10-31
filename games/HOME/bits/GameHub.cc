#include "GameHub.h"

namespace home {

 GameHub::GameHub(const std::filesystem::path& asset_directory)
 : gf::SingleSceneManager("H.O.M.E.", { 1600, 900 })
 , resource_hub(asset_directory)
 {
   set_scene(&world_scene);
 }

}
