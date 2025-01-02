// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_ID_H
#define GF_ID_H

#include <cstdint>

#include <string_view>

namespace gf {

  // NOLINTNEXTLINE(performance-enum-size)
  enum class Id : uint64_t { };

  constexpr Id id(uint64_t id)
  {
    return static_cast<Id>(id);
  }

  constexpr Id InvalidId = id(0);

  constexpr Id hash_string(const char* str, std::size_t sz)
  {
    uint64_t value = UINT64_C(0xcbf29ce484222325);

    for (std::size_t i = 0; i < sz; ++i) {
      value ^= static_cast<unsigned char>(str[i]);
      value *= UINT64_C(0x100000001b3);
    }

    return id(value);
  }

  constexpr Id hash_string(std::string_view str)
  {
    return hash_string(str.data(), str.size());
  }

  namespace literals {

    constexpr Id operator""_id(const char* str, std::size_t sz)
    {
      return hash_string(str, sz);
    }

  } // namespace literals

} // namespace gf

#endif // GF_ID_H
