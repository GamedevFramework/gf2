// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_RESOURCE_LOADER_H
#define GF_RESOURCE_LOADER_H

#include <filesystem>
#include <functional>
#include <memory>
#include <type_traits>

#include "ResourceContext.h"

namespace gf {

  template<typename T>
  using ResourceLoader = std::conditional_t<
    std::is_empty_v<ResourceContext<T>>,
    std::function<std::unique_ptr<T>(const std::filesystem::path&)>,
    std::function<std::unique_ptr<T>(const std::filesystem::path&, const ResourceContext<T>&)>
  >;

  template<typename T, typename U>
  ResourceLoader<T> loader_for(U& loader)
  {
    if constexpr (std::is_empty_v<ResourceContext<T>>) {
      return [&loader](const std::filesystem::path& path) {
        return loader.template operator()<T>(path);
      };
    } else {
      return [&loader](const std::filesystem::path& path, const ResourceContext<T>& context) {
        return loader.template operator()<T>(path, context);
      };
    }
  }

} // namespace gf

#endif // GF_RESOURCE_LOADER_H
