#include <gf2/core/ResourceLoader.h>
#include <gf2/core/ResourceRegistry.h>
#include <gf2/core/ResourceLoaders.h>

struct MyResource {
  MyResource(const std::filesystem::path&)
  {
  }
};


void doc_ResourceLoader() {

  // tag::loader_for[]
  gf::ResourceRegistry<MyResource> registry;

  gf::FileLoader loader; // FileLoader is a generic loader
  registry.add_loader(gf::loader_for<MyResource>(loader));
  // end::loader_for[]

}
