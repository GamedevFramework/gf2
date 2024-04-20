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
#include "ResourceContext.h"
#include "ResourceManager.h"
#include "TypeTraits.h"

namespace gf {
  namespace details {
    template<typename T, typename = std::void_t<>>
    inline constexpr bool HasPrimitive = false;

    template<typename T>
    inline constexpr bool HasPrimitive<T, std::void_t<typename T::Primitive>> = true;

    template<typename T, typename = std::void_t<>>
    inline constexpr bool HasBundle = false;

    template<typename T>
    inline constexpr bool HasBundle<T, std::void_t<decltype(T::bundle(std::declval<std::filesystem::path>()))>> = true;

    template<typename T, typename = std::void_t<>>
    inline constexpr bool HasBundleWithContext = false;

    template<typename T>
    inline constexpr bool HasBundleWithContext<T, std::void_t<decltype(T::bundle(std::declval<std::filesystem::path>(), std::declval<ResourceContext<T>>()))>> = true;
  } // namespace details

  enum class ResourceAction : uint8_t {
    Load,
    Unload,
  };

  class GF_CORE_API ResourceBundle {
  public:
    ResourceBundle() = default;

    template<typename F, typename = PreventHiddenCopyMove<F, ResourceBundle>>
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
            if constexpr (details::HasPrimitive<T>) {
              handle<typename T::Primitive>(path, manager, action);
            }

            if constexpr (details::HasBundle<T>) {
              T::bundle(path).load_from(manager);
            }

            manager->load<T>(path);
          }
          break;

        case ResourceAction::Unload:
          {
            manager->unload<T>(path);

            if constexpr (details::HasBundle<T>) {
              T::bundle(path).unload_from(manager);
            }

            if constexpr (details::HasPrimitive<T>) {
              handle<typename T::Primitive>(path, manager, action);
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
            if constexpr (details::HasPrimitive<T>) {
              if constexpr (details::HasContext<typename T::Primitive>) {
                handle<typename T::Primitive>(path, context, manager, action);
              } else {
                handle<typename T::Primitive>(path, manager, action);
              }
            }

            if constexpr (details::HasBundleWithContext<T>) {
              T::bundle(path, context).load_from(manager);
            }

            manager->load<T>(path, context);
          }
          break;

        case ResourceAction::Unload:
          {
            manager->unload<T>(path);

            if constexpr (details::HasBundleWithContext<T>) {
              T::bundle(path, context).unload_from(manager);
            }

            if constexpr (details::HasPrimitive<T>) {
              if constexpr (details::HasContext<typename T::Primitive>) {
                handle<typename T::Primitive>(path, context, manager, action);
              } else {
                handle<typename T::Primitive>(path, manager, action);
              }
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
