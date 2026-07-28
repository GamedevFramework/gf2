// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Dice.h>

namespace gf {

  int Dice::roll(gf::Random* random) const {
    int result = m_modifier;

    for (int i = 0; i < m_count; ++i) {
      result += random->compute_uniform_integer(1, m_faces);
    }

    return result;
  }
}
