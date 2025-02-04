// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_Z_STRING_H
#define GF_Z_STRING_H

#include <cassert>

#include <string>
#include <string_view>

namespace gf {

  class ZString {
  public:
    constexpr ZString() = default;

    constexpr ZString(const char* string)
    : m_data(string)
    , m_size(string == nullptr ? 0 : std::char_traits<char>::length(string))
    {
    }

    constexpr bool empty() const
    {
      return m_size == 0;
    }

    constexpr std::size_t size() const
    {
      return m_size;
    }

    constexpr const char* data() const
    {
      return m_data;
    }

    constexpr const char* c_str() const
    {
      return m_data;
    }

    constexpr const char* begin() const
    {
      return m_data;
    }

    constexpr const char* end() const
    {
      return m_data + m_size;
    }

    constexpr char operator[](std::size_t index) const
    {
      assert(index < m_size);
      return m_data[index];
    }

    constexpr operator std::string_view() const
    {
      return { m_data, m_size };
    }

    std::string to_string() const
    {
      return { m_data, m_size };
    }

  private:
    const char* m_data = nullptr;
    std::size_t m_size = 0;
  };

}

#endif // GF_Z_STRING_H
