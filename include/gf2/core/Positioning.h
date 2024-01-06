// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_POSITIONING_H
#define GF_POSITIONING_H

#include "CoreApi.h"
#include "Orientation.h"
#include "Vec2.h"

namespace gf {

  class GF_CORE_API Positioning {
  public:
    Positioning(Vec2I surface_size);

    Vec2I surface_size() const
    {
      return m_surface_size;
    }

    Vec2F center() const;

    Vec2F relative_point(Vec2F percent) const;
    Vec2F relative_size(Vec2F percent) const;

    float character_size(float percent) const;

    Vec2F absolute_point(Vec2F distance, Orientation orientation) const;

  private:
    Vec2I m_surface_size;
  };

}

#endif // GF_POSITIONING_H
