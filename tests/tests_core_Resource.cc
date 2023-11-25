#include <type_traits>

#include <gf2/core/ResourceBundle.h>
#include <gf2/core/ResourceLoaders.h>
#include <gf2/core/ResourceManager.h>
#include <gf2/core/ResourceRegistry.h>

#include "gtest/gtest.h"

namespace {
  struct DummyResource {
    DummyResource(std::size_t x)
    : x(x)
    {
    }

    DummyResource([[maybe_unused]] const std::filesystem::path& path)
    {
    }

    DummyResource(gf::InputStream& input)
    {
      input.read(gf::bytes(&x));
    }

    std::size_t x = 0;
  };

  struct DummyCompositeResource {

    DummyCompositeResource([[maybe_unused]] const std::filesystem::path& path)
    {
    }

    static gf::ResourceBundle bundle(const std::filesystem::path& path)
    {
      gf::ResourceBundle bundle([path](gf::ResourceBundle* bundle, auto manager, auto action) {
        bundle->handle<DummyResource>("sub" / path, manager, action);
      });
      return bundle;
    }
  };

  struct DummyContextResource {
    struct Context {
      bool dummy = true;
    };

    DummyContextResource([[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] const Context& context)
    {
    }
  };

  struct DummyPrimitive {
    DummyPrimitive([[maybe_unused]] const std::filesystem::path& path)
    {
    }

  };

  struct DummyPrimitiveResource {
    using Primitive = DummyPrimitive;

    DummyPrimitiveResource([[maybe_unused]] const std::filesystem::path& path)
    {
    }

  };

  struct DummyLoader {

    template<typename T>
    std::unique_ptr<T> operator()(const std::filesystem::path& path, [[maybe_unused]] const gf::ResourceContext<T>& context = {})
    {
      if constexpr (std::is_empty_v<gf::ResourceContext<T>>) {
        return std::make_unique<T>(path);
      } else {
        return std::make_unique<T>(path, context);
      }
    }
  };

} // namespace

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

  [[maybe_unused]] auto* resource = registry.load("foo");

  EXPECT_TRUE(registry.loaded("foo"));
}

TEST(ResourceTest, RegistryLoadTwice) {
  DummyLoader loader;

  gf::ResourceRegistry<DummyResource> registry;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_FALSE(registry.loaded("foo"));

  [[maybe_unused]] auto* resource0 = registry.load("foo");
  [[maybe_unused]] auto* resource1 = registry.load("foo");

  EXPECT_TRUE(registry.loaded("foo"));
}

TEST(ResourceTest, RegistryUnloadOnce) {
  DummyLoader loader;

  gf::ResourceRegistry<DummyResource> registry;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_FALSE(registry.loaded("foo"));

  [[maybe_unused]] auto* resource = registry.load("foo");

  EXPECT_TRUE(registry.loaded("foo"));

  registry.unload("foo");

  EXPECT_FALSE(registry.loaded("foo"));
}

TEST(ResourceTest, RegistryUnloadTwice) {
  DummyLoader loader;

  gf::ResourceRegistry<DummyResource> registry;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_FALSE(registry.loaded("foo"));

  [[maybe_unused]] auto* resource0 = registry.load("foo");
  [[maybe_unused]] auto* resource1 = registry.load("foo");

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

  EXPECT_ANY_THROW(registry.unload("foo")); // NOLINT

  EXPECT_FALSE(registry.loaded("foo"));
}

TEST(ResourceTest, RegistryGet) {
  DummyLoader loader;

  gf::ResourceRegistry<DummyResource> registry;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_FALSE(registry.loaded("foo"));

  [[maybe_unused]] auto* resource0 = registry.load("foo");
  [[maybe_unused]] auto* resource1 = registry.get("foo");

  EXPECT_EQ(resource0, resource1);
  EXPECT_TRUE(registry.loaded("foo"));

  registry.unload("foo");

  EXPECT_FALSE(registry.loaded("foo"));
}

TEST(ResourceTest, RegistryInvalidGet) {
  DummyLoader loader;

  gf::ResourceRegistry<DummyResource> registry;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_FALSE(registry.loaded("foo"));

  EXPECT_ANY_THROW({ // NOLINT
    [[maybe_unused]] auto* resource0 = registry.get("foo");
  });

  EXPECT_FALSE(registry.loaded("foo"));
}

TEST(ResourceTest, ManagerConstructor) {
  gf::ResourceManager manager;
}

TEST(ResourceTest, ManagerWithRegistry) {
  gf::ResourceRegistry<DummyResource> registry;

  gf::ResourceManager manager;
  manager.add_registry(&registry);

  DummyLoader loader;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_NO_THROW(manager.load<DummyResource>("foo")); // NOLINT
  EXPECT_TRUE(registry.loaded("foo"));
}

TEST(ResourceTest, ManagerInvalidRegistry) {
  gf::ResourceManager manager;
  EXPECT_ANY_THROW(manager.get<DummyResource>("unknown")); // NOLINT
}

TEST(ResourceTest, ManagerAcquire) { // NOLINT
  gf::ResourceRegistry<DummyResource> registry;

  gf::ResourceManager manager;
  manager.add_registry(&registry);

  DummyLoader loader;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  EXPECT_NO_THROW(manager.acquire<DummyResource>("foo")); // NOLINT
  EXPECT_NO_THROW(manager.acquire<DummyResource>("foo")); // NOLINT
  EXPECT_NO_THROW(manager.unload<DummyResource>("foo")); // NOLINT
}

TEST(ResourceTest, MemoryLoader) {
  std::size_t raw_resource = 42;

  gf::MemoryLoader loader;
  loader.add_buffer("foo", gf::bytes(&raw_resource));

  gf::ResourceRegistry<DummyResource> registry;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  auto* reference = registry.load("foo");

  EXPECT_EQ(reference->x, raw_resource);
}

TEST(ResourceTest, BundleEmpty) { // NOLINT
  gf::ResourceManager manager;

  gf::ResourceBundle bundle;

  EXPECT_NO_THROW(bundle.load_from(&manager)); // NOLINT
  EXPECT_NO_THROW(bundle.unload_from(&manager)); // NOLINT
}

TEST(ResourceTest, BundleLoadUnload) {
  gf::ResourceRegistry<DummyResource> registry;

  DummyLoader loader;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  gf::ResourceManager manager;
  manager.add_registry(&registry);

  gf::ResourceBundle bundle([](gf::ResourceBundle* bundle, auto manager, auto action) {
    bundle->handle<DummyResource>("foo", manager, action);
    bundle->handle<DummyResource>("bar", manager, action);
  });

  bundle.load_from(&manager);

  EXPECT_TRUE(registry.loaded("foo"));
  EXPECT_TRUE(registry.loaded("bar"));

  bundle.unload_from(&manager);

  EXPECT_FALSE(registry.loaded("foo"));
  EXPECT_FALSE(registry.loaded("bar"));
}

TEST(ResourceTest, BundleMultiple) {
  gf::ResourceRegistry<DummyResource> registry;

  DummyLoader loader;
  registry.add_loader(gf::loader_for<DummyResource>(loader));

  gf::ResourceManager manager;
  manager.add_registry(&registry);

  gf::ResourceBundle bundle1([](gf::ResourceBundle* bundle, auto manager, auto action) {
    bundle->handle<DummyResource>("foo", manager, action);
    bundle->handle<DummyResource>("bar", manager, action);
  });

  gf::ResourceBundle bundle2([](gf::ResourceBundle* bundle, auto manager, auto action) {
    bundle->handle<DummyResource>("foo", manager, action);
  });

  bundle1.load_from(&manager);

  EXPECT_TRUE(registry.loaded("foo"));
  EXPECT_TRUE(registry.loaded("bar"));

  bundle2.load_from(&manager);
  bundle1.unload_from(&manager);

  EXPECT_TRUE(registry.loaded("foo"));
  EXPECT_FALSE(registry.loaded("bar"));

  bundle2.unload_from(&manager);

  EXPECT_FALSE(registry.loaded("foo"));
  EXPECT_FALSE(registry.loaded("bar"));
}

TEST(ResourceTest, BundleComposite) {
  gf::ResourceRegistry<DummyCompositeResource> registry_for_composite;
  gf::ResourceRegistry<DummyResource> registry_for_single;

  DummyLoader loader;
  registry_for_composite.add_loader(gf::loader_for<DummyCompositeResource>(loader));
  registry_for_single.add_loader(gf::loader_for<DummyResource>(loader));

  gf::ResourceManager manager;
  manager.add_registry(&registry_for_composite);
  manager.add_registry(&registry_for_single);

  gf::ResourceBundle bundle([](gf::ResourceBundle* bundle, auto manager, auto action) {
    bundle->handle<DummyCompositeResource>("foo", manager, action);
  });

  bundle.load_from(&manager);

  EXPECT_TRUE(registry_for_composite.loaded("foo"));
  EXPECT_TRUE(registry_for_single.loaded("sub/foo"));

  bundle.unload_from(&manager);

  EXPECT_FALSE(registry_for_composite.loaded("foo"));
  EXPECT_FALSE(registry_for_single.loaded("sub/foo"));

}

TEST(ResourceTest, BundleContext) {
  gf::ResourceRegistry<DummyContextResource> registry;

  gf::ResourceManager manager;
  manager.add_registry(&registry);

  DummyLoader loader;
  registry.add_loader(gf::loader_for<DummyContextResource>(loader));

  DummyContextResource::Context context;

  gf::ResourceBundle bundle([&context](gf::ResourceBundle* bundle, auto manager, auto action) {
    bundle->handle<DummyContextResource>("foo", context, manager, action);
  });

  bundle.load_from(&manager);

  EXPECT_TRUE(registry.loaded("foo"));

  bundle.unload_from(&manager);

  EXPECT_FALSE(registry.loaded("foo"));
}

TEST(ResourceTest, BundlePrimitive) {
  gf::ResourceRegistry<DummyPrimitiveResource> registry0;
  gf::ResourceRegistry<DummyPrimitive> registry1;

  gf::ResourceManager manager;
  manager.add_registry(&registry0);
  manager.add_registry(&registry1);

  DummyLoader loader;
  registry0.add_loader(gf::loader_for<DummyPrimitiveResource>(loader));
  registry1.add_loader(gf::loader_for<DummyPrimitive>(loader));

  gf::ResourceBundle bundle([](gf::ResourceBundle* bundle, auto manager, auto action) {
    bundle->handle<DummyPrimitiveResource>("foo", manager, action);
  });

  bundle.load_from(&manager);

  EXPECT_TRUE(registry0.loaded("foo"));
  EXPECT_TRUE(registry1.loaded("foo"));

  bundle.unload_from(&manager);

  EXPECT_FALSE(registry0.loaded("foo"));
  EXPECT_FALSE(registry1.loaded("foo"));
}
