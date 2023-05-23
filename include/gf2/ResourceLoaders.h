// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_RESOURCE_LOADERS_H
#define GF_RESOURCE_LOADERS_H

#include <map>

#include "CoreApi.h"
#include "ResourceContext.h"
#include "ResourceLoader.h"
#include "Span.h"
#include "Streams.h"
#include "Tarball.h"

namespace gf {

  class GF_CORE_API FileLoader {
  public:
    void add_search_directory(std::filesystem::path directory);
    std::filesystem::path search(const std::filesystem::path& relative_path);

    template<typename T>
    std::unique_ptr<T> operator()(const std::filesystem::path& path, const ResourceContext<T>& context = {})
    {
      auto absolute_path = search(path);

      if (absolute_path.empty()) {
        return nullptr;
      }

      if constexpr (std::is_empty_v<gf::ResourceContext<T>>) {
        return std::make_unique<T>(absolute_path);
      } else {
        return std::make_unique<T>(absolute_path, context);
      }
    }

  private:
    std::vector<std::filesystem::path> m_search_directories;
  };

  class GF_CORE_API MemoryLoader {
  public:
    void add_buffer(std::filesystem::path path, Span<uint8_t> buffer);
    Span<uint8_t> search(const std::filesystem::path& relative_path);

    template<typename T>
    std::unique_ptr<T> operator()(const std::filesystem::path& path, const ResourceContext<T>& context = {})
    {
      auto buffer = search(path);

      if (buffer.empty()) {
        return nullptr;
      }

      MemoryInputStream input(buffer);

      if constexpr (std::is_empty_v<gf::ResourceContext<T>>) {
        return std::make_unique<T>(input);
      } else {
        return std::make_unique<T>(input, context);
      }
    }

  private:
    std::map<std::filesystem::path, Span<uint8_t>> m_buffers;
  };

  class GF_CORE_API TarballLoader {
  public:
    TarballLoader(const std::filesystem::path& tarball_path);

    std::vector<uint8_t> search(const std::filesystem::path& relative_path);

    template<typename T>
    std::unique_ptr<T> operator()(const std::filesystem::path& path, const ResourceContext<T>& context = {})
    {
      auto buffer = search(path);

      if (buffer.empty()) {
        return nullptr;
      }

      BufferInputStream input(gf::ref(buffer));

      if constexpr (std::is_empty_v<gf::ResourceContext<T>>) {
        return std::make_unique<T>(input);
      } else {
        return std::make_unique<T>(input, context);
      }
    }

  private:
    Tarball m_tarball;
  };

} // namespace gf

#endif // GF_RESOURCE_LOADERS_H
