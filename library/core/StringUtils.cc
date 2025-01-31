// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/StringUtils.h>

#include <cmath>

#include <algorithm>

namespace gf {

  std::string nice_num(float num, float precision)
  {
    const float accpow = std::floor(std::log10(precision));

    int digits = 0;

    if (num < 0) {
      digits = static_cast<int>(std::fabs((num / std::pow(10, accpow)) - 0.5f));
    } else {
      digits = static_cast<int>(std::fabs((num / std::pow(10, accpow)) + 0.5f));
    }

    std::string result;

    if (digits > 0) {
      int curpow = static_cast<int>(accpow);

      for (int i = 0; i < curpow; ++i) {
        result += '0';
      }

      while (digits > 0) {
        const char adigit = static_cast<char>((digits % 10) + '0');

        if (curpow == 0 && !result.empty()) {
          result += '.';
          result += adigit;
        } else {
          result += adigit;
        }

        digits /= 10;
        curpow += 1;
      }

      for (int i = curpow; i < 0; ++i) {
        result += '0';
      }

      if (curpow <= 0) {
        result += ".0";
      }

      if (num < 0) {
        result += '-';
      }

      std::reverse(result.begin(), result.end());
    } else {
      result = "0";
    }

    return result;
  }

  std::string escape_string(std::string_view text)
  {
    std::string result;

    for (auto c : text) {
      switch (c) {
        case '\n':
          result.append("\\n");
          break;
        case '\t':
          result.append("\\t");
          break;
        case '\"':
          result.append("\\\"");
          break;
        case '\\':
          result.append("\\\\");
          break;
        default:
          result.push_back(c);
          break;
      }
    }

    return result;
  }

  namespace {
    bool is_delimiter(char c, std::string_view delimiters)
    {
      return std::any_of(delimiters.begin(), delimiters.end(), [c](char d) { return c == d; });
    }
  }

  std::vector<std::string_view> split_string(std::string_view string, std::string_view delimiters)
  {
    const std::size_t size = string.size();
    std::size_t i = 0;
    std::vector<std::string_view> result;

    while (i < size) {
      while (i < size && is_delimiter(string[i], delimiters)) {
        ++i;
      }

      if (i == size) {
        break;
      }

      const char* start = string.data() + i;
      size_t length = 0;

      while (i < size && !is_delimiter(string[i], delimiters)) {
        ++i;
        ++length;
      }

      result.emplace_back(start, length);
    }

    return result;
  }

  std::u32string to_utf32(std::string_view utf8)
  {
    CodepointRange range = codepoints(utf8);
    return { range.begin(), range.end() };
  }

  std::string to_utf8(std::u32string_view utf32)
  {
    std::string utf8;

    for (char32_t c : utf32) {
      if (c < 0x80) {
        utf8.push_back(static_cast<char>(c & 0x7F));
      } else if (c < 0x800) {
        utf8.push_back(static_cast<char>(((c >> 6) & 0x1F) | 0xC0));
        utf8.push_back(static_cast<char>(((c >> 0) & 0x3F) | 0x80));
      } else if (c < 0x10000) {
        utf8.push_back(static_cast<char>(((c >> 12) & 0x0F) | 0xE0));
        utf8.push_back(static_cast<char>(((c >>  6) & 0x3F) | 0x80));
        utf8.push_back(static_cast<char>(((c >>  0) & 0x3F) | 0x80));
      } else if (c < 0x110000) {
        utf8.push_back(static_cast<char>(((c >> 18) & 0x07) | 0xF0));
        utf8.push_back(static_cast<char>(((c >> 12) & 0x3F) | 0x80));
        utf8.push_back(static_cast<char>(((c >>  6) & 0x3F) | 0x80));
        utf8.push_back(static_cast<char>(((c >>  0) & 0x3F) | 0x80));
      } else {
        assert(false);
      }
    }

    return utf8;
  }

}
