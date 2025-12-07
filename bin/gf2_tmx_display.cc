#include <cstdlib>

#include <filesystem>
#include <iostream>

#include <gf2/core/ResourceBundle.h>
#include <gf2/core/ResourceLoader.h>
#include <gf2/core/ResourceLoaders.h>
#include <gf2/core/ResourceManager.h>
#include <gf2/core/ResourceRegistry.h>
#include <gf2/core/RichMapResource.h>
#include <gf2/core/TiledMap.h>
#include <gf2/core/Zoom.h>

#include <gf2/graphics/Entity.h>
#include <gf2/graphics/RenderManager.h>
#include <gf2/graphics/RenderRecorder.h>
#include <gf2/graphics/RichMap.h>
#include <gf2/graphics/RichMapRenderer.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>

namespace {
  constexpr auto ViewSize = gf::vec(1600, 900);

  class MapEntity : public gf::Entity {
  public:
    MapEntity(gf::RichMap* map, gf::RenderManager* render_manager)
    : m_renderer(map, render_manager)
    {
    }

    void render(gf::RenderRecorder& recorder) override
    {
      auto geometries = m_renderer.select_geometry("");

      if (geometries.empty()) {
        return;
      }

      for (auto& geometry : geometries) {
        gf::RenderObject object = {};
        object.priority = priority();
        object.geometry = geometry;
        object.transform = gf::Identity3F;
        recorder.record(object);
      }
    }

    const gf::RichMapRenderer* map_renderer() const
    {
      return &m_renderer;
    }

  private:
    gf::RichMapRenderer m_renderer;
  };

  class TmxScene : public gf::Scene {
  public:
    TmxScene(gf::RichMap* map, gf::RenderManager* render_manager)
    : m_zoom(world_camera())
    , m_map_entity(map, render_manager)
    {

      add_world_entity(&m_map_entity);

      auto bounds = m_map_entity.map_renderer()->grid().compute_bounds();

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
    // scene manager
    gf::SingleSceneManager scene_manager("gf2_tmx_display", ViewSize);

    // resources
    const std::filesystem::path path = argv[1];
    const std::filesystem::path directory = std::filesystem::absolute(path.parent_path());
    const std::filesystem::path filename = path.filename();

    gf::FileLoader file_loader;
    file_loader.add_search_directory(directory);

    gf::ResourceRegistry<gf::TiledMap> tiled_map_registry;
    tiled_map_registry.add_loader(gf::loader_for<gf::TiledMap>(file_loader));

    gf::ResourceRegistry<gf::GpuTexture> texture_registry;
    texture_registry.add_loader(gf::loader_for<gf::GpuTexture>(file_loader));

    gf::ResourceRegistry<gf::RichMap> rich_map_registry;
    rich_map_registry.add_loader(gf::loader_for<gf::RichMap>(file_loader));

    gf::ResourceManager resource_manager;
    resource_manager.add_registry(&tiled_map_registry);
    resource_manager.add_registry(&texture_registry);
    resource_manager.add_registry(&rich_map_registry);

    gf::ResourceBundle bundle([filename, &scene_manager](gf::ResourceBundle* bundle, gf::ResourceManager* resource_manager, gf::ResourceAction action) {
      bundle->handle<gf::RichMap>(filename, { scene_manager.render_manager(), resource_manager }, resource_manager, action);
    });

    bundle.load_from(&resource_manager);

    // scene

    TmxScene scene(resource_manager.get<gf::RichMap>(filename), scene_manager.render_manager());

    return scene_manager.run(&scene);
  } catch (...) {
    return EXIT_FAILURE;
  }
}
