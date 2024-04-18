// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_STRING_UTILS_H
#define GF_STRING_UTILS_H

#include <cassert>
#include <cstdint>

#include <string>
#include <vector>

#include "CoreApi.h"

namespace gf {

  GF_CORE_API std::string nice_num(float num, float precision);

  GF_CORE_API std::string escape_string(std::string_view text);

  GF_CORE_API std::vector<std::string_view> split_string(std::string_view string, std::string_view delimiters);

  inline std::vector<std::string_view> split_in_paragraphs(std::string_view text)
  {
    return split_string(text, "\n");
  }

  inline std::vector<std::string_view> split_in_words(std::string_view text)
  {
    return split_string(text, " \t");
  }

  inline std::vector<std::string_view> split_path(std::string_view path_like)
  {
    return split_string(path_like, "/");
  }

  class GF_CORE_API CodepointRange {
  public:
    class Iterator {
    public:
      using difference_type = std::ptrdiff_t;
      using value_type = char32_t;
      using pointer = value_type;
      using reference = value_type;
      using iterator_category = std::forward_iterator_tag;

      constexpr Iterator(const char* ptr) noexcept
      : m_current(ptr)
      {
      }

      void swap(Iterator& other) noexcept { std::swap(m_current, other.m_current); }
      constexpr reference operator*() const noexcept { return decode(); }
      constexpr pointer operator->() const noexcept { return decode(); }

      constexpr Iterator& operator++() noexcept
      {
        step();
        return *this;
      }

      constexpr Iterator operator++(int) noexcept
      {
        Iterator copy = *this;
        step();
        return copy;
      }

      constexpr bool operator!=(const Iterator& other) const noexcept { return m_current != other.m_current; }
      constexpr bool operator==(const Iterator& other) const noexcept { return m_current == other.m_current; }

    private:
      const char* m_current;

      constexpr char32_t decode() const noexcept
      {
        char32_t codepoint = 0;
        const uint8_t c = m_current[0];

        if ((c & 0b10000000) == 0b00000000) {
          codepoint = c & 0b01111111;
        } else if ((c & 0b11100000) == 0b11000000) {
          codepoint = c & 0b00011111;
          codepoint = (codepoint << 6) + (m_current[1] & 0b00111111);
        } else if ((c & 0b11110000) == 0b11100000) {
          codepoint = c & 0b00001111;
          codepoint = (codepoint << 6) + (m_current[1] & 0b00111111);
          codepoint = (codepoint << 6) + (m_current[2] & 0b00111111);
        } else {
          assert((c & 0b11111000) == 0b11110000);
          codepoint = c & 0b00000111;
          codepoint = (codepoint << 6) + (m_current[1] & 0b00111111);
          codepoint = (codepoint << 6) + (m_current[2] & 0b00111111);
          codepoint = (codepoint << 6) + (m_current[3] & 0b00111111);
        }

        return codepoint;
      }

      constexpr void step() noexcept
      {
        const uint8_t c = m_current[0];

        if ((c & 0b10000000) == 0b00000000) {
          m_current += 1;
        } else if ((c & 0b11100000) == 0b11000000) {
          m_current += 2;
        } else if ((c & 0b11110000) == 0b11100000) {
          m_current += 3;
        } else {
          assert((c & 0b11111000) == 0b11110000);
          m_current += 4;
        }
      }
    };

    constexpr CodepointRange(std::string_view string) noexcept
    : m_string(string)
    {
    }

    constexpr Iterator begin() const noexcept
    {
      return { m_string.data() }; // NOLINT(bugprone-suspicious-stringview-data-usage)
    }

    constexpr Iterator end() const noexcept
    {
      return { m_string.data() + m_string.size() };
    }

  private:
    std::string_view m_string;
  };

  constexpr CodepointRange codepoints(std::string_view string) noexcept
  {
    return { string };
  }

}

#endif // GF_STRING_UTILS_H
