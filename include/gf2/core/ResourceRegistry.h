#ifndef GF_RESOURCE_REGISTRY_H
#define GF_RESOURCE_REGISTRY_H

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
      return m_cache.find(path) != m_cache.end();
    }

    T* get(const std::filesystem::path& path)
    {
      if (auto cache_iterator = m_cache.find(path); cache_iterator != m_cache.end()) {
        auto& [_, counted] = *cache_iterator;
        // no ref increase
        return counted.pointer.get();
      }

      Log::fatal("Resource not already loaded: '{}'", path.string());
    }

    T* load(const std::filesystem::path& path, const ResourceContext<T>& context = {})
    {
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

      Log::fatal("Resource not loaded: '{}'", path.string());
    }

    void unload(const std::filesystem::path& path)
    {
      if (auto cache_iterator = m_cache.find(path); cache_iterator != m_cache.end()) {
        auto& [_, counted] = *cache_iterator;
        --counted.count;

        if (counted.count == 0) {
          m_cache.erase(cache_iterator);
        }
      } else {
        Log::fatal("Resource already unloaded: '{}'", path.string());
      }
    }

  private:
    struct Counted {
      std::unique_ptr<T> pointer;
      std::size_t count = 1;
    };

    std::vector<ResourceLoader<T>> m_loaders;
    std::map<std::filesystem::path, Counted> m_cache;
  };

} // namespace gf

#endif // GF_RESOURCE_REGISTRY_H
