// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/ConsoleEffect.h>

#include <cassert>

namespace gf {

  Color ConsoleEffect::compute_color(Color existing, Color proposed) const
  {
    return combine_colors(proposed, existing, m_mode, m_operation);
  }

}
