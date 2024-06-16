#include <gf2/core/Image.h>
#include <gf2/core/ResourceLoaders.h>
#include <gf2/core/ResourceManager.h>
#include <gf2/core/ResourceRegistry.h>
#include "gf2/framework/ResourceSystem.h"

void doc_simple_load()
{
// tag::simple_load[]
  gf::ResourceManager resource_manager;

  // load an image (reference couting)
  gf::Image* image = resource_manager.load<gf::Image>("path/to/image.png");

  // ...

  // use the image (no reference counting)
  gf::Image* same_image = resource_manager.get<gf::Image>("path/to/image.png");
// end::simple_load[]

  (void) image;
  (void) same_image;
}

void doc_manual_configuration()
{
// tag::manual_configuration[]
  // define a resource loader
  gf::FileLoader file_loader;
  file_loader.add_search_directory("path/to/assets");

  // define a registry and add the loader
  gf::ResourceRegistry<gf::Image> image_registry;
  image_registry.add_loader(gf::loader_for<gf::Image>(file_loader));

  // define the resource manager and add the registry
  gf::ResourceManager resource_manager;
  resource_manager.add_registry(&image_registry);
// end::manual_configuration[]
}

void doc_resource_system()
{
// tag::resource_system[]
  gf::ResourceSystem resource_system("path/to/assets");

  // load an image
  auto* image = resource_system.load<gf::Image>("path/to/image.png");

  // ...

  // use the image
  auto* same_image = resource_system.get<gf::Image>("path/to/image.png");
// end::resource_system[]

  (void) image;
  (void) same_image;
}

// tag::sfml_loader_definition[]
class SfmlLoader {
public:

  template<typename T>
  std::unique_ptr<T> operator()(const std::filesystem::path& path)
  {
    auto resource = std::make_unique<T>();
    resource->loadFromFile(path.string());
    return resource;
  }

};
// end::sfml_loader_definition[]

namespace sf {

  class SoundBuffer {
  public:

    // NOLINTNEXTLINE(readability-identifier-naming)
    void loadFromFile([[maybe_unused]] const std::string& filename)
    {
    }

  };

}

void doc_load_sfml()
{
// tag::sfml_loader_use[]
  SfmlLoader sfml_loader;

  gf::ResourceRegistry<sf::SoundBuffer> sound_buffer_registry;
  sound_buffer_registry.add_loader(gf::loader_for<sf::SoundBuffer>(sfml_loader));

  gf::ResourceSystem resource_system("path/to/assets");
  resource_system.add_registry(&sound_buffer_registry);

  auto* sound_buffer = resource_system.load<sf::SoundBuffer>("path/to/sound_buffer");

// end::sfml_loader_use[]

  (void) sound_buffer;
}

