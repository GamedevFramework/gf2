// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_RESOURCE_LOADER_H
#define GF_RESOURCE_LOADER_H

#include <filesystem>
#include <functional>
#include <memory>

namespace gf {

  template<typename T>
  using ResourceLoader = std::function<std::unique_ptr<T>(const std::filesystem::path&)>;

  template<typename T, typename U>
  ResourceLoader<T> loader_for(U& loader)
  {
    return [&loader](const std::filesystem::path& path) {
      return loader.template operator()<T>(path);
    };
  }

} // namespace gf

#endif // GF_RESOURCE_LOADER_H
