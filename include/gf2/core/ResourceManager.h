// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_RESOURCE_MANAGER_H
#define GF_RESOURCE_MANAGER_H

#include <cassert>
#include <cstddef>

#include <any>
#include <filesystem>
#include <map>
#include <mutex>
#include <typeindex>

#include "CoreApi.h"
#include "Log.h"
#include "ResourceContext.h"
#include "ResourceRegistry.h"

namespace gf {

  class GF_CORE_API ResourceManager {
  public:

    void add_registry(std::nullptr_t registry) = delete;

    template<typename T>
    void add_registry(ResourceRegistry<T>* registry)
    {
      const std::lock_guard<std::recursive_mutex> lock(m_mutex);
      assert(registry != nullptr);
      [[maybe_unused]] auto [iterator, inserted] = m_resources.emplace(std::type_index(typeid(T)), registry);
      assert(inserted);
    }

    template<typename T>
    T* get(const std::filesystem::path& path)
    {
      const std::lock_guard<std::recursive_mutex> lock(m_mutex);
      ResourceRegistry<T>* registry = search_registry<T>();
      return registry->get(path);
    }

    template<typename T>
    T* acquire(const std::filesystem::path& path, const ResourceContext<T>& context = {})
    {
      const std::lock_guard<std::recursive_mutex> lock(m_mutex);
      ResourceRegistry<T>* registry = search_registry<T>();

      if (registry->loaded(path)) {
        return registry->get(path);
      }

      return registry->load(path, context);
    }

    template<typename T>
    T* load(const std::filesystem::path& path, const ResourceContext<T>& context = {})
    {
      const std::lock_guard<std::recursive_mutex> lock(m_mutex);
      ResourceRegistry<T>* registry = search_registry<T>();
      return registry->load(path, context);
    }

    template<typename T>
    void unload(const std::filesystem::path& path)
    {
      const std::lock_guard<std::recursive_mutex> lock(m_mutex);
      ResourceRegistry<T>* registry = search_registry<T>();
      registry->unload(path);
    }

  private:
    template<typename T>
    ResourceRegistry<T>* search_registry()
    {
      if (auto resources_iterator = m_resources.find(std::type_index(typeid(T))); resources_iterator != m_resources.end()) {
        auto& [type_index, any_registry] = *resources_iterator;
        assert(any_registry.type() == typeid(ResourceRegistry<T>*));
        return std::any_cast<ResourceRegistry<T>*>(any_registry);
      }

      Log::fatal("No registry for this type of resource.");
    }

    std::recursive_mutex m_mutex;
    std::map<std::type_index, std::any> m_resources;
  };

} // namespace gf

#endif // GF_RESOURCE_MANAGER_H
