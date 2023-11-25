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
#include "gf2/core/ResourceContext.h"

namespace gf {
  namespace details {
    template<typename T, typename = std::void_t<>>
    inline constexpr bool HasPrimitive = false;

    template<typename T>
    inline constexpr bool HasPrimitive<T, std::void_t<typename T::Primitive>> = true;

    template<typename T, typename = std::void_t<>>
    inline constexpr bool HasBundle = false;

    template<typename T>
    inline constexpr bool HasBundle<T, std::void_t<decltype(std::declval<T>().bundle())>> = true;
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
            if constexpr (details::HasPrimitive<T>) {
              handle<typename T::Primitive>(path, manager, action);
            }

            [[maybe_unused]] auto* ptr = manager->load<T>(path);

            if constexpr (details::HasBundle<T>) {
              ptr->bundle().load_from(manager);
            }
          }
          break;

        case ResourceAction::Unload:
          {
            if constexpr (details::HasBundle<T>) {
              auto* ptr = manager->get<T>(path);
              ptr->bundle().unload_from(manager);
            }

            manager->unload<T>(path);

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
              handle<typename T::Primitive>(path, context, manager, action);
            }

            [[maybe_unused]] auto* ptr = manager->load<T>(path, context);

            if constexpr (details::HasBundle<T>) {
              ptr->bundle().load_from(manager);
            }
          }
          break;

        case ResourceAction::Unload:
          {
            if constexpr (details::HasBundle<T>) {
              auto* ptr = manager->get<T>(path);
              ptr->bundle().unload_from(manager);
            }

            manager->unload<T>(path);

            if constexpr (details::HasPrimitive<T>) {
              handle<typename T::Primitive>(path, context, manager, action);
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
