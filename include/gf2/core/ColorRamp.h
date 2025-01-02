// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_COLOR_RAMP_H
#define GF_COLOR_RAMP_H

#include <map>

#include "Color.h"
#include "CoreApi.h"

namespace gf {

  class GF_CORE_API ColorRamp {
  public:
    void clear()
    {
      m_mapping.clear();
    }

    bool empty() const
    {
      return m_mapping.empty();
    }

    void add_color_stop(float offset, const Color& color);
    Color compute_color(float offset) const;

  private:
    float m_min = 0.0f;
    float m_max = 0.0f;
    std::map<float, Color> m_mapping;
  };

}

#endif // GF_COLOR_RAMP_H
