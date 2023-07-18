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
    inline constexpr bool HasBundle = false;

    template<typename T>
    inline constexpr bool HasBundle<T, std::void_t<decltype(std::declval<T>().bundle())>> = true;
  } // namespace details

  class GF_CORE_API ResourceBundle {
  public:
    enum class Action : uint8_t {
      Load,
      Unload,
    };

    ResourceBundle() = default;

    template<typename F>
    ResourceBundle(F&& callback)
    : m_callback(std::forward<F>(callback))
    {
    }

    template<typename F>
    void set_callback(F&& callback)
    {
      m_callback = std::forward<F>(callback);
    }

    void load_from(ResourceManager& manager);
    void unload_from(ResourceManager& manager);

    template<typename T>
    void handle(const std::filesystem::path& path, ResourceManager& manager, Action action)
    {
      static_assert(std::is_empty_v<ResourceContext<T>>, "Resource context should be empty.");

      switch (action) {
        case Action::Load:
          {
            [[maybe_unused]] T& ref = manager.load<T>(path);

            if constexpr (details::HasBundle<T>) {
              ref.bundle().load_from(manager);
            }
          }
          break;

        case Action::Unload:
          {
            if constexpr (details::HasBundle<T>) {
              T& ref = manager.get<T>(path);
              ref.bundle().unload_from(manager);
            }

            manager.unload<T>(path);
          }
          break;
      }
    }

    template<typename T>
    void handle(const std::filesystem::path& path, const ResourceContext<T>& context, ResourceManager& manager, Action action)
    {
      static_assert(!std::is_empty_v<ResourceContext<T>>, "Resource context should be non-empty.");

      switch (action) {
        case Action::Load:
          {
            [[maybe_unused]] T& ref = manager.load<T>(path, context);

            if constexpr (details::HasBundle<T>) {
              ref.bundle().load_from(manager);
            }
          }
          break;

        case Action::Unload:
          {
            if constexpr (details::HasBundle<T>) {
              T& ref = manager.get<T>(path);
              ref.bundle().unload_from(manager);
            }

            manager.unload<T>(path);
          }
          break;
      }
    }

  private:
    std::function<void(ResourceBundle&, ResourceManager&, Action)> m_callback;
  };

} // namespace gf

#endif // GF_RESOURCE_BUNDLE_H
