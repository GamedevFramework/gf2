// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_RAW_GEOMETRY_H
#define GF_RAW_GEOMETRY_H

#include <cstdint>

#include <algorithm>
#include <vector>

#include <gf2/core/Rect.h>

#include "Vertex.h"

namespace gf {

  struct RawGeometry {
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    template<typename T>
    void merge_with(const T& other)
    {
      const std::size_t offset = vertices.size();
      vertices.insert(vertices.end(), other.vertices.begin(), other.vertices.end());
      std::transform(other.indices.begin(), other.indices.end(), std::back_inserter(indices), [offset](uint16_t index) {
        return static_cast<uint16_t>(index + offset);
      });
    }

    RectF compute_bounds() const;
  };

}

#endif // GF_RAW_GEOMETRY_H
