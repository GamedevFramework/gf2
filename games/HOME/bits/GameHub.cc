#include "GameHub.h"

namespace home {

  GameHub::GameHub(const std::filesystem::path& asset_directory)
  : gf::SceneSystem("H.O.M.E.", { 1600, 900 }, asset_directory)
  , m_world_async(render_manager())
  {
    auto splash_bundle = m_splash_data.bundle(this);
    splash_bundle.load_from(resource_manager());

    m_splash_scene = std::make_unique<SplashScene>(this, m_splash_data);

    m_world_async.run_async([this]() {
      auto world_bundle = m_world_data.bundle(this);
      world_bundle.load_from(resource_manager());

      m_world_scene = std::make_unique<WorldScene>(this, m_world_data);
    });

    push_scene(splash_scene());
  }

  bool GameHub::is_world_loaded()
  {
    return m_world_async.finished();
  }

}
