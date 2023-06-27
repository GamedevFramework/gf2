#ifndef GF_TARBALL_H
#define GF_TARBALL_H

#include <cstdint>

#include <filesystem>
#include <vector>

#include "CoreApi.h"

#include <zlib.h>

namespace gf {

  class GF_CORE_API Tarball {
  public:
    Tarball(const std::filesystem::path& file);
    Tarball(const Tarball&) = delete;
    Tarball(Tarball&& other) noexcept;
    ~Tarball();

    Tarball& operator=(const Tarball&) = delete;
    Tarball& operator=(Tarball&& other) noexcept;

    std::vector<uint8_t> extract(const std::filesystem::path& path);

    std::vector<std::filesystem::path> entries() const;

  private:
    struct Entry {
      std::filesystem::path path;
      long offset = 0;
      std::size_t size = 0;
    };

    void read_entries();
    std::vector<uint8_t> extract_entry(const Entry& entry);

    gzFile m_file = nullptr;

    std::vector<Entry> m_entries;
  };

} // namespace gf

#endif // GF_TARBALL_H
