// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_RESOURCE_BUNDLE_H
#define GF_RESOURCE_BUNDLE_H

#include <cstdint>

#include <filesystem>
#include <functional>
#include <type_traits>
#include <utility>

#include "CoreApi.h"
#include "ResourceManager.h"

namespace gf {
  namespace details {
    template<typename T, typename = std::void_t<>>
    inline constexpr bool HasStaticBundle = false;

    template<typename T>
    inline constexpr bool HasStaticBundle<T, std::void_t<decltype(T::bundle(std::declval<std::filesystem::path>()))>> = true;

    template<typename T, typename = std::void_t<>>
    inline constexpr bool HasStaticBundleWithContext = false;

    template<typename T>
    inline constexpr bool HasStaticBundleWithContext<T, std::void_t<decltype(T::bundle(std::declval<std::filesystem::path>(), std::declval<ResourceContext<T>>()))>> = true;
  } // namespace details

  enum class ResourceAction : uint8_t {
    Load,
    Unload,
  };

  class GF_CORE_API ResourceBundle {
  public:
    ResourceBundle() = default;

    template<typename F, typename = std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<F>>, ResourceBundle>>>
    explicit ResourceBundle(F&& callback)
    : m_callback(std::forward<F>(callback))
    {
    }

    template<typename F>
    void set_callback(F&& callback)
    {
      m_callback = std::forward<F>(callback);
    }

    void load_from(ResourceManager* manager);
    void unload_from(ResourceManager* manager);

    template<typename T>
    void handle(const std::filesystem::path& path, ResourceManager* manager, ResourceAction action)
    {
      static_assert(std::is_empty_v<ResourceContext<T>>, "Resource context should be empty.");

      switch (action) {
        case ResourceAction::Load:
          {
            if constexpr (details::HasStaticBundle<T>) {
              T::bundle(path).load_from(manager);
            }

            manager->load<T>(path);
          }
          break;

        case ResourceAction::Unload:
          {
            manager->unload<T>(path);

            if constexpr (details::HasStaticBundle<T>) {
              T::bundle(path).unload_from(manager);
            }
          }
          break;
      }
    }

    template<typename T>
    void handle(const std::filesystem::path& path, const ResourceContext<T>& context, ResourceManager* manager, ResourceAction action)
    {
      static_assert(!std::is_empty_v<ResourceContext<T>>, "Resource context should be non-empty.");

      switch (action) {
        case ResourceAction::Load:
          {
            if constexpr (details::HasStaticBundleWithContext<T>) {
              T::bundle(path, context).load_from(manager);
            }

            manager->load<T>(path, context);
          }
          break;

        case ResourceAction::Unload:
          {
            manager->unload<T>(path);

            if constexpr (details::HasStaticBundleWithContext<T>) {
              T::bundle(path, context).unload_from(manager);
            }
          }
          break;
      }
    }

  private:
    std::function<void(ResourceBundle*, ResourceManager*, ResourceAction)> m_callback;
  };

} // namespace gf

#endif // GF_RESOURCE_BUNDLE_H
