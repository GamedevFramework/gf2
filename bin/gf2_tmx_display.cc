#include <cstdlib>

#include <filesystem>
#include <iostream>

#include <gf2/core/ResourceBundle.h>
#include <gf2/core/ResourceLoader.h>
#include <gf2/core/ResourceLoaders.h>
#include <gf2/core/ResourceManager.h>
#include <gf2/core/ResourceRegistry.h>
#include <gf2/core/TiledMapData.h>

#include <gf2/graphics/Entity.h>
#include <gf2/graphics/RenderManager.h>
#include <gf2/graphics/RenderRecorder.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>
#include <gf2/graphics/TiledMap.h>
#include <gf2/graphics/Zoom.h>

namespace {
  constexpr auto ViewSize = gf::vec(1600, 900);

  class MapEntity : public gf::Entity {
  public:
    void set_map(gf::TiledMap* map)
    {
      m_map = map;
    }

    void render(gf::RenderRecorder& recorder) override
    {
      if (m_map == nullptr) {
        return;
      }

      auto geometries = m_map->select_geometry("");

      for (auto& geometry : geometries) {
        gf::RenderObject object = {};
        object.priority = priority();
        object.geometry = geometry;
        object.transform = gf::Identity3F;
        recorder.record(object);
      }
    }

  private:
    gf::TiledMap* m_map = nullptr;
  };

  class TmxScene : public gf::Scene {
  public:
    TmxScene(const std::filesystem::path& path, gf::RenderManager* render_manager)
    : m_zoom(world_camera())
    {
      auto directory = std::filesystem::absolute(path.parent_path());
      auto filename = path.filename();

      m_file_loader.add_search_directory(directory);

      m_texture_registry.add_loader(gf::loader_for<gf::Texture>(m_file_loader));
      m_map_resource_registry.add_loader(gf::loader_for<gf::TiledMapResource>(m_file_loader));
      m_map_registry.add_loader(gf::loader_for<gf::TiledMap>(m_file_loader));

      m_resource_manager.add_registry(&m_texture_registry);
      m_resource_manager.add_registry(&m_map_resource_registry);
      m_resource_manager.add_registry(&m_map_registry);

      gf::ResourceBundle bundle([&](gf::ResourceBundle* bundle, gf::ResourceManager* resources, gf::ResourceAction action) {
        bundle->handle<gf::TiledMap>(filename, { render_manager, resources }, resources, action);
      });

      bundle.load_from(&m_resource_manager);

      auto* map = m_resource_manager.get<gf::TiledMap>(filename);

      m_map_entity.set_map(map);
      add_world_entity(&m_map_entity);

      auto bounds = map->grid().compute_bounds();

      set_world_size(ViewSize);
      set_world_center(bounds.center());
    }

  private:
    void do_process_event(const gf::Event& event) override
    {
      m_zoom.set_surface_size(surface_size());
      m_zoom.process_event(event);
    }

    gf::Zoom m_zoom;

    gf::FileLoader m_file_loader;
    gf::ResourceRegistry<gf::Texture> m_texture_registry;
    gf::ResourceRegistry<gf::TiledMapResource> m_map_resource_registry;
    gf::ResourceRegistry<gf::TiledMap> m_map_registry;
    gf::ResourceManager m_resource_manager;

    MapEntity m_map_entity;
  };

}

int main(int argc, char* argv[])
{
  if (argc == 1) {
    std::cout << "Usage: gf2_tmx_display <file>\n";
    return EXIT_FAILURE;
  }

  try {
    gf::SingleSceneManager scene_manager("gf2_tmx_display", ViewSize);
    TmxScene scene(argv[1], scene_manager.render_manager());

    return scene_manager.run(&scene);
  } catch (...) {
    return EXIT_FAILURE;
  }
}
