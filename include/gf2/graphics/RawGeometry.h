// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_RAW_GEOMETRY_H
#define GF_RAW_GEOMETRY_H

#include <cstdint>

#include <algorithm>
#include <map>
#include <vector>

#include <gf2/core/Rect.h>
#include <gf2/core/Span.h>

#include "GraphicsApi.h"
#include "Vertex.h"

namespace gf {

  struct GF_GRAPHICS_API RawGeometry {
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

  struct GF_GRAPHICS_API RawSplitGeometry {
    std::map<uint32_t, RawGeometry> map;

    void add_quad(uint32_t texture_index, StaticSpan<const Vertex, 4> vertices);

    RawGeometry merge() const;
  };

}

#endif // GF_RAW_GEOMETRY_H
