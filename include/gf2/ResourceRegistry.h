#ifndef GF_RESOURCE_REGISTRY_H
#define GF_RESOURCE_REGISTRY_H

#include <filesystem>
#include <map>
#include <stdexcept>
#include <vector>

#include "Log.h"
#include "Ref.h"
#include "ResourceLoader.h"

namespace gf {

  template<typename T>
  class ResourceRegistry {
  public:

    void add_loader(ResourceLoader<T> loader)
    {
      m_loaders.push_back(std::move(loader));
    }

    bool loaded(const std::filesystem::path& path)
    {
      return m_cache.find(path) != m_cache.end();
    }

    Ref<T> get(const std::filesystem::path& path)
    {
      if (auto cache_iterator = m_cache.find(path); cache_iterator != m_cache.end()) {
        auto& [_, counted] = *cache_iterator;
        // no ref increase
        return gf::ref(*counted.pointer);
      }

      Log::error("Resource not already loaded: '{}'", path.string());
      throw std::runtime_error("Resource not already loaded");
    }

    Ref<T> load(const std::filesystem::path& path)
    {
      if (auto cache_iterator = m_cache.find(path); cache_iterator != m_cache.end()) {
        auto& [_, counted] = *cache_iterator;
        ++counted.count;
        return gf::ref(*counted.pointer);
      }

      for (auto& loader : m_loaders) {
        auto pointer = loader(path);

        if (!pointer) {
          continue;
        }

        auto [iterator, inserted] = m_cache.emplace(path, Counted{ std::move(pointer), 1 });

        if (!inserted) {
          Log::error("Resource not inserted in the registry: '{}'", path.string());
          throw std::runtime_error("Resource not inserted in the registry.");
        }

        return gf::ref(*iterator->second.pointer);
      }

      Log::error("Resource not loaded: '{}'", path.string());
      throw std::runtime_error("Resource not loaded.");
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
        Log::error("Resource already unloaded: '{}'", path.string());
        throw std::runtime_error("Resource already unloaded.");
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