#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <cassert>

#include <any>
#include <filesystem>
#include <map>
#include <stdexcept>
#include <typeindex>

#include "CoreApi.h"
#include "Log.h"
#include "ResourceRegistry.h"

namespace gf {

  class GF_CORE_API ResourceManager {
  public:

    template<typename T>
    void add_registry(Ref<ResourceRegistry<T>> registry)
    {
      [[maybe_unused]] auto [iterator, inserted] = m_resources.emplace(std::type_index(typeid(T)), registry);
      assert(inserted);
    }

    template<typename T>
    Ref<T> get(const std::filesystem::path& path)
    {
      ResourceRegistry<T>& registry = search_registry<T>();
      return registry.get(path);
    }

    template<typename T>
    Ref<T> acquire(const std::filesystem::path& path)
    {
      ResourceRegistry<T>& registry = search_registry<T>();

      if (registry.loaded(path)) {
        return registry.get(path);
      }

      return registry.load(path);
    }

    template<typename T>
    Ref<T> load(const std::filesystem::path& path)
    {
      ResourceRegistry<T>& registry = search_registry<T>();
      return registry.load(path);
    }

    template<typename T>
    void unload(const std::filesystem::path& path)
    {
      ResourceRegistry<T>& registry = search_registry<T>();
      registry.unload(path);
    }

  private:
    template<typename T>
    ResourceRegistry<T>& search_registry()
    {
      if (auto resources_iterator = m_resources.find(std::type_index(typeid(T))); resources_iterator != m_resources.end()) {
        auto& [type_index, any_registry] = *resources_iterator;
        assert(any_registry.type() == typeid(Ref<ResourceRegistry<T>>));
        return std::any_cast<Ref<ResourceRegistry<T>>>(any_registry);
      }

      Log::error("No registry for this type of resource.");
      throw std::runtime_error("No registry for this type of resource.");
    }

    std::map<std::type_index, std::any> m_resources;
  };

} // namespace gf

#endif // RESOURCE_MANAGER_H
