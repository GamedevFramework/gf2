// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_DICE_H
#define GF_DICE_H

#include <cassert>
#include <climits>
#include <string_view>

#include "CoreApi.h"
#include "Random.h"

namespace gf {

  class GF_CORE_API Dice {
  public:
    constexpr explicit Dice(int faces, int count = 1, int modifier = 0)
    : m_faces(faces)
    , m_count(count)
    , m_modifier(modifier)
    {
    }

    constexpr Dice(std::string_view str)
    : m_faces(0)
    , m_count(0)
    , m_modifier(0)
    {
      std::size_t i = 0;
      std::size_t sz = str.size();

      while (i < sz && '0' <= str[i] && str[i] <= '9') {
        m_count = (m_count * 10) + (str[i] - '0');
        ++i;
      }

      if (i == sz) {
        return;
      }

      if (str[i] != 'd' && str[i] != 'D') {
        return;
      }

      if (m_count == 0) {
        m_count = 1;
      }

      ++i; // eat 'd'

      while (i < sz && '0' <= str[i] && str[i] <= '9') {
        m_faces = (m_faces * 10) + (str[i] - '0');
        ++i;
      }

      if (i == sz) {
        return;
      }

      if (str[i] != '+') {
        return;
      }

      ++i; // eat '+'

      while (i < sz && '0' <= str[i] && str[i] <= '9') {
        m_modifier = (m_modifier * 10) + (str[i] - '0');
        ++i;
      }
    }

    constexpr int faces() const {
      return m_faces;
    }

    constexpr int count() const {
      return m_count;
    }

    constexpr int modifier() const {
      return m_modifier;
    }

    int roll(gf::Random* random) const;

  private:
    int m_faces;
    int m_count;
    int m_modifier;
  };

  namespace literals {

    constexpr gf::Dice operator""_dice(const char *str, std::size_t sz) {
      return { std::string_view(str, sz) };
    }

  }
}

#endif // GF_DICE_H
