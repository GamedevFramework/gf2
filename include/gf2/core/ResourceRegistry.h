// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_RESOURCE_REGISTRY_H
#define GF_RESOURCE_REGISTRY_H

#include <cassert>

#include <filesystem>
#include <map>
#include <vector>

#include "Log.h"
#include "ResourceContext.h"
#include "ResourceLoader.h"

namespace gf {

  template<typename T>
  class ResourceRegistry {
  public:

    void add_loader(ResourceLoader<T> loader)
    {
      m_loaders.push_back(std::move(loader));
    }

    bool loaded(const std::filesystem::path& path) const
    {
      if (path.is_absolute()) {
        std::filesystem::path base_path = path.parent_path();

        while (base_path.has_relative_path()) {
          const std::filesystem::path relative_path = std::filesystem::relative(path, base_path);

          if (try_loaded(relative_path)) {
            return true;
          }

          base_path = base_path.parent_path();
        }
      } else {
        return try_loaded(path);
      }

      return false;
    }

    T* get(const std::filesystem::path& path) const
    {
      if (path.is_absolute()) {
        std::filesystem::path base_path = path.parent_path();

        while (base_path.has_relative_path()) {
          const std::filesystem::path relative_path = std::filesystem::relative(path, base_path);

          if (auto* resource = try_get(relative_path); resource != nullptr) {
            return resource;
          }

          base_path = base_path.parent_path();
        }
      } else {
        if (auto* resource = try_get(path); resource != nullptr) {
          return resource;
        }
      }

      Log::fatal("Resource not already loaded: '{}'", path.string());
    }

    T* load(const std::filesystem::path& path, const ResourceContext<T>& context = {})
    {
      if (path.is_absolute()) {
        std::filesystem::path base_path = path.parent_path();

        while (base_path.has_relative_path()) {
          const std::filesystem::path relative_path = std::filesystem::relative(path, base_path);

          if (auto* resource = try_load(relative_path, context); resource != nullptr) {
            return resource;
          }

          base_path = base_path.parent_path();
        }
      } else {
        if (auto* resource = try_load(path, context); resource != nullptr) {
          return resource;
        }
      }

      Log::fatal("Resource not loaded: '{}'", path.string());
    }

    void unload(const std::filesystem::path& path)
    {
      if (path.is_absolute()) {
        std::filesystem::path base_path = path.parent_path();

        while (base_path.has_relative_path()) {
          const std::filesystem::path relative_path = std::filesystem::relative(path, base_path);

          if (try_unload(relative_path)) {
            return;
          }

          base_path = base_path.parent_path();
        }
      } else {
        if (try_unload(path)) {
          return;
        }
      }

      Log::fatal("Resource already unloaded: '{}'", path.string());
    }

  private:
    bool try_loaded(const std::filesystem::path& path) const
    {
      return m_cache.find(path) != m_cache.end();
    }

    T* try_get(const std::filesystem::path& path) const
    {
      assert(path.is_relative());

      if (auto cache_iterator = m_cache.find(path); cache_iterator != m_cache.end()) {
        auto& [_, counted] = *cache_iterator;
        // no ref increase
        return counted.pointer.get();
      }

      return nullptr;
    }

    T* try_load(const std::filesystem::path& path, const ResourceContext<T>& context = {})
    {
      assert(path.is_relative());

      if (auto cache_iterator = m_cache.find(path); cache_iterator != m_cache.end()) {
        auto& [_, counted] = *cache_iterator;
        ++counted.count;
        return counted.pointer.get();
      }

      for (auto& loader : m_loaders) {
        std::unique_ptr<T> pointer;

        if constexpr (std::is_empty_v<ResourceContext<T>>) {
          pointer = loader(path);
        } else {
          pointer = loader(path, context);
        }

        if (!pointer) {
          continue;
        }

        auto [iterator, inserted] = m_cache.emplace(path, Counted{ std::move(pointer), 1 });

        if (!inserted) {
          Log::fatal("Resource not inserted in the registry: '{}'", path.string());
        }

        auto& [_, counted] = *iterator;
        return counted.pointer.get();
      }

      return nullptr;
    }

    bool try_unload(const std::filesystem::path& path)
    {
      if (auto cache_iterator = m_cache.find(path); cache_iterator != m_cache.end()) {
        auto& [_, counted] = *cache_iterator;
        --counted.count;

        if (counted.count == 0) {
          m_cache.erase(cache_iterator);
        }

        return true;
      }

      return false;
    }

    struct Counted {
      std::unique_ptr<T> pointer;
      std::size_t count = 1;
    };

    std::vector<ResourceLoader<T>> m_loaders;
    std::map<std::filesystem::path, Counted> m_cache;
  };

} // namespace gf

#endif // GF_RESOURCE_REGISTRY_H
