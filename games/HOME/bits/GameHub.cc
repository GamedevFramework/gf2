#include "GameHub.h"

namespace home {

  GameHub::GameHub(const std::filesystem::path& asset_directory)
  : gf::SceneManager("H.O.M.E.", { 1600, 900 })
  , m_resource_hub(asset_directory)
  {
    auto splash_bundle = m_splash_data.bundle(this);
    splash_bundle.load_from(resource_manager());

    m_splash_scene = std::make_unique<SplashScene>(this, m_splash_data);

    m_world_loaded = std::async(std::launch::async, [this]() {
      auto world_bundle = m_world_data.bundle(this);
      world_bundle.load_from(resource_manager());

      m_world_scene = std::make_unique<WorldScene>(this, m_world_data);
      return true;
    });

    push_scene(splash_scene());
  }

  bool GameHub::is_world_loaded()
  {
    if (m_world_loaded.valid() && m_world_loaded.wait_for(std::chrono::seconds::zero()) == std::future_status::ready) {
      if (!m_world_loaded.get()) {
        gf::Log::fatal("Error while loading world!");
      }

      return true;
    }

    return false;
  }

}
