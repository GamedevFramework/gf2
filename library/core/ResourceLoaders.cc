// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/ResourceLoaders.h>

#include <cassert>

#include <gf2/core/Log.h>

namespace gf {

  /*
   * FileLoader
   */

  void FileLoader::add_search_directory(std::filesystem::path directory)
  {
    if (!directory.is_absolute()) {
      std::filesystem::path absolute = std::filesystem::current_path() / directory;
      Log::warning("Directory '{}' is relative, using '{}' instead.", directory.string(), absolute.string());
      std::swap(directory, absolute);
    }

    if (!std::filesystem::is_directory(directory)) {
      Log::warning("Directory not found: '{}'", directory.string());
      return;
    }

    Log::info("Added a new search directory: '{}'", directory.string());
    m_search_directories.push_back(std::move(directory));
  }

  std::filesystem::path FileLoader::search(const std::filesystem::path& relative_path)
  {
    assert(relative_path.is_relative());

    for (const std::filesystem::path& base : m_search_directories) {
      std::filesystem::path absolute_path = base / relative_path;

      if (std::filesystem::is_regular_file(absolute_path)) {
        Log::info("Found a resource file ['{}']: '{}'", base.string(), relative_path.string());
        return absolute_path;
      }
    }

    return {};
  }

  /*
   * MemoryLoader
   */

  void MemoryLoader::add_buffer(std::filesystem::path path, Span<uint8_t> buffer)
  {
    [[maybe_unused]] auto [iterator, inserted] = m_buffers.emplace(std::move(path), buffer);
    assert(inserted);
  }

  Span<uint8_t> MemoryLoader::search(const std::filesystem::path& relative_path)
  {
    if (auto iterator = m_buffers.find(relative_path); iterator != m_buffers.end()) {
      return iterator->second;
    }

    return {};
  }

  /*
   * TarballLoader
   */

  TarballLoader::TarballLoader(const std::filesystem::path& tarball_path)
  : m_tarball(tarball_path)
  {
  }

  std::vector<uint8_t> TarballLoader::search(const std::filesystem::path& relative_path)
  {
    return m_tarball.extract(relative_path);
  }

} // namespace gf
