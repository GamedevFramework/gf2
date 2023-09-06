// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/core/Tarball.h>
// clang-format on

#include <cassert>

#include <algorithm>
#include <array>
#include <functional>
#include <iterator>
#include <string_view>
#include <utility>

#include <gf2/core/Log.h>

namespace gf {

  namespace {
    struct TarHeader {
      char name[100];
      char mode[8];
      char uid[8];
      char gid[8];
      char size[12];
      char mtime[12];
      char chksum[8];
      char typeflag;
      char linkname[100];
      char magic[6];
      char version[2];
      char uname[32];
      char gname[32];
      char devmajor[8];
      char devminor[8];
      char prefix[155];

      bool check_magic() const
      {
        return magic[0] == 'u' && magic[1] == 's' && magic[2] == 't' && magic[3] == 'a' && magic[4] == 'r';
      }
    };

    union TarRecord {
      TarHeader header;
      std::array<uint8_t, 512> data;
    };

    static_assert(sizeof(TarRecord) == 512, "Weird record size!");

  } // namespace

  Tarball::Tarball(const std::filesystem::path& file)
  {
    m_file = gzopen(file.string().c_str(), "rb");

    if (m_file == nullptr) {
      Log::warning("Could not open the tarball: '{}'", file.string());
      return;
    }

    read_entries();
  }

  Tarball::Tarball(Tarball&& other) noexcept
  : m_file(std::exchange(other.m_file, nullptr))
  , m_entries(std::move(other.m_entries))
  {
  }

  Tarball::~Tarball()
  {
    if (m_file != nullptr) {
      gzclose(m_file);
    }
  }

  Tarball& Tarball::operator=(Tarball&& other) noexcept
  {
    std::swap(m_file, other.m_file);
    m_entries = std::move(other.m_entries);
    return *this;
  }

  std::vector<uint8_t> Tarball::extract(const std::filesystem::path& path)
  {
    if (m_file == nullptr) {
      return {};
    }

    for (auto& entry : m_entries) {
      if (entry.path == path) {
        return extract_entry(entry);
      }
    }

    return {};
  }

  std::vector<std::filesystem::path> Tarball::entries() const
  {
    std::vector<std::filesystem::path> entries;
    std::transform(m_entries.begin(), m_entries.end(), std::back_inserter(entries), std::mem_fn(&Entry::path));
    return entries;
  }

  // format description: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/pax.html#tag_20_92_13_06
  void Tarball::read_entries()
  {
    assert(m_file != nullptr);

    while (gzeof(m_file) == 0) {
      TarRecord record = {};
      const std::size_t read = gzfread(&record, sizeof(uint8_t), sizeof(TarRecord), m_file);

      if (read == 0) {
        break;
      }

      if (std::all_of(record.data.begin(), record.data.end(), [](uint8_t byte) { return byte == 0; })) {
        // skip all zero records
        continue;
      }

      // we have a header
      assert(record.header.check_magic());

      std::size_t size = 0;

      for (const char c : record.header.size) {
        if (c == '\0') {
          break;
        }

        size = size * 8 + (c - '0');
      }

      if (record.header.typeflag != '0' && record.header.typeflag != 0) {
        // not a file
        assert(size == 0);
        continue;
      }

      Entry entry;

      if (record.header.prefix[0] == '\0') {
        entry.path = record.header.name;
      } else {
        entry.path = record.header.prefix;
        entry.path /= record.header.name;
      }

      entry.offset = gztell(m_file);
      assert(entry.offset % sizeof(TarRecord) == 0);
      entry.size = size;
      m_entries.push_back(std::move(entry));

      const std::size_t record_count = (size + 511) / 512;
      gzseek(m_file, static_cast<long>(record_count * sizeof(TarRecord)), SEEK_CUR);
    }
  }

  std::vector<uint8_t> Tarball::extract_entry(const Entry& entry)
  {
    assert(m_file != nullptr);

    std::vector<uint8_t> content;
    content.reserve(entry.size);

    gzseek(m_file, entry.offset, SEEK_SET);
    std::array<uint8_t, 512> buffer = {};
    std::size_t size = 0;

    while (gzeof(m_file) == 0 && size < entry.size) {
      const std::size_t wanted = std::min(buffer.size(), entry.size - size);
      const std::size_t read = gzfread(buffer.data(), sizeof(uint8_t), wanted, m_file);
      assert(read == wanted);
      content.insert(content.end(), buffer.data(), buffer.data() + read);
      size += read;
    }

    return content;
  }

} // namespace gf
