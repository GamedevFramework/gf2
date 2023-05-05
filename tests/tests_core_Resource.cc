#include <gf2/ResourceLoaders.h>
#include <gf2/ResourceManager.h>
#include <gf2/ResourceRegistry.h>

#include <type_traits>

#include "gtest/gtest.h"

namespace {
  struct DummyResource {
    DummyResource(std::size_t x)
    : x(x)
    {
    }

    DummyResource(const std::filesystem::path&)
    {
    }

    DummyResource(gf::InputStream& input)
    : x(0)
    {
      input.read(gf::bytes(&x));
    }

    std::size_t x;
  };

  struct DummyLoader {

    template<typename T>
    std::unique_ptr<T> operator()(const std::filesystem::path& path) {
      return std::make_unique<T>(path);
    }

  };

}


TEST(ResourceTest, RegistryConstructor) {
  gf::ResourceRegistry<DummyResource> registry;
  EXPECT_FALSE(registry.loaded("unknown"));
}

TEST(ResourceTest, RegistryWithLoader) {
  DummyLoader loader;

  gf::ResourceRegistry<DummyResource> registry;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_FALSE(registry.loaded("unknown"));
}

TEST(ResourceTest, RegistryLoadOnce) {
  DummyLoader loader;

  gf::ResourceRegistry<DummyResource> registry;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_FALSE(registry.loaded("foo"));

  [[maybe_unused]]
  auto resource = registry.load("foo");

  EXPECT_TRUE(registry.loaded("foo"));
}

TEST(ResourceTest, RegistryLoadTwice) {
  DummyLoader loader;

  gf::ResourceRegistry<DummyResource> registry;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_FALSE(registry.loaded("foo"));

  [[maybe_unused]]
  auto resource0 = registry.load("foo");
  [[maybe_unused]]
  auto resource1 = registry.load("foo");

  EXPECT_TRUE(registry.loaded("foo"));
}

TEST(ResourceTest, RegistryUnloadOnce) {
  DummyLoader loader;

  gf::ResourceRegistry<DummyResource> registry;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_FALSE(registry.loaded("foo"));

  [[maybe_unused]]
  auto resource = registry.load("foo");

  EXPECT_TRUE(registry.loaded("foo"));

  registry.unload("foo");

  EXPECT_FALSE(registry.loaded("foo"));
}

TEST(ResourceTest, RegistryUnloadTwice) {
  DummyLoader loader;

  gf::ResourceRegistry<DummyResource> registry;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_FALSE(registry.loaded("foo"));

  [[maybe_unused]]
  auto resource0 = registry.load("foo");
  [[maybe_unused]]
  auto resource1 = registry.load("foo");

  EXPECT_TRUE(registry.loaded("foo"));

  registry.unload("foo");

  EXPECT_TRUE(registry.loaded("foo"));

  registry.unload("foo");

  EXPECT_FALSE(registry.loaded("foo"));
}

TEST(ResourceTest, RegistryInvalidUnload) {
  DummyLoader loader;

  gf::ResourceRegistry<DummyResource> registry;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_FALSE(registry.loaded("foo"));

  EXPECT_ANY_THROW(registry.unload("foo"));

  EXPECT_FALSE(registry.loaded("foo"));
}

TEST(ResourceTest, RegistryGet) {
  DummyLoader loader;

  gf::ResourceRegistry<DummyResource> registry;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_FALSE(registry.loaded("foo"));

  [[maybe_unused]]
  auto resource0 = registry.load("foo");
  [[maybe_unused]]
  auto resource1 = registry.get("foo");

  EXPECT_TRUE(registry.loaded("foo"));

  registry.unload("foo");

  EXPECT_FALSE(registry.loaded("foo"));
}

TEST(ResourceTest, RegistryInvalidGet) {
  DummyLoader loader;

  gf::ResourceRegistry<DummyResource> registry;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_FALSE(registry.loaded("foo"));

  EXPECT_ANY_THROW({
    [[maybe_unused]]
    auto resource0 = registry.get("foo");
  });

  EXPECT_FALSE(registry.loaded("foo"));
}

TEST(ResourceTest, ManagerConstructor) {
  gf::ResourceManager manager;
}

TEST(ResourceTest, ManagerWithRegistry) {
  gf::ResourceRegistry<DummyResource> registry;

  gf::ResourceManager manager;
  manager.add_registry(gf::ref(registry));

  DummyLoader loader;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_NO_THROW(manager.load<DummyResource>("foo"));
}

TEST(ResourceTest, ManagerInvalidRegistry) {
  gf::ResourceManager manager;
  EXPECT_ANY_THROW(manager.get<DummyResource>("unknown"));
}

TEST(ResourceTest, ManagerAcquire) {
  gf::ResourceRegistry<DummyResource> registry;

  gf::ResourceManager manager;
  manager.add_registry(gf::ref(registry));

  DummyLoader loader;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_NO_THROW(manager.acquire<DummyResource>("foo"));
  EXPECT_NO_THROW(manager.acquire<DummyResource>("foo"));
  EXPECT_NO_THROW(manager.unload<DummyResource>("foo"));
}

TEST(ResourceTest, MemoryLoader) {
  DummyResource resource(42);

  gf::MemoryLoader loader;
  loader.add_buffer("foo", gf::bytes(&resource));

  gf::ResourceRegistry<DummyResource> registry;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  auto reference = registry.load("foo");

  EXPECT_EQ(reference.get().x, resource.x);
}
