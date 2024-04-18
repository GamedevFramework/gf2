// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/ColorRamp.h>

#include <cassert>

namespace gf {

  void ColorRamp::add_color_stop(float offset, const Color& color)
  {
    if (empty()) {
      m_min = m_max = offset;
    } else {
      m_min = std::min(offset, m_min);
      m_max = std::max(offset, m_max);
    }

    m_mapping.emplace(offset, color);
  }

  Color ColorRamp::compute_color(float offset) const
  {
    if (m_mapping.empty()) {
      return White;
    }

    if (offset < m_min) {
      return m_mapping.begin()->second;
    }

    if (offset > m_max) {
      return m_mapping.rbegin()->second;
    }

    auto iterator = m_mapping.lower_bound(offset);
    assert(iterator != m_mapping.end());
    auto [t2, c2] = *iterator;

    if (iterator == m_mapping.begin()) {
      return c2;
    }

    --iterator;
    auto [t1, c1] = *iterator;

    return gf::lerp(c1, c2, (offset - t1) / (t2 - t1));
  }

}
