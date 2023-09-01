// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/StringUtils.h>
// clang-format on

#include <cmath>

#include <algorithm>

namespace gf {

  std::string nice_num(float num, float precision)
  {
    const float accpow = std::floor(std::log10(precision));

    int digits = 0;

    if (num < 0) {
      digits = static_cast<int>(std::fabs(num / std::pow(10, accpow) - 0.5f));
    } else {
      digits = static_cast<int>(std::fabs(num / std::pow(10, accpow) + 0.5f));
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

    std::vector<std::string_view> split(std::string_view str, std::string_view delimiters)
    {
      const std::size_t size = str.size();
      std::size_t i = 0;
      std::vector<std::string_view> result;

      while (i < size) {
        while (i < size && is_delimiter(str[i], delimiters)) {
          ++i;
        }

        if (i == size) {
          break;
        }

        const char* start = str.data() + i;
        size_t length = 0;

        while (i < size && !is_delimiter(str[i], delimiters)) {
          ++i;
          ++length;
        }

        result.emplace_back(start, length);
      }

      return result;
    }
  }

  std::vector<std::string_view> split_in_paragraphs(std::string_view str)
  {
    return split(str, "\n");
  }

  std::vector<std::string_view> split_in_words(std::string_view str)
  {
    return split(str, " \t");
  }

  std::vector<std::string_view> split_path(std::string_view path_like)
  {
    return split(path_like, "/");
  }

}
