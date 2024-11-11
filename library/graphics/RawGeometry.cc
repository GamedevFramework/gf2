// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/RawGeometry.h>

#include <numeric>

namespace gf {

  /*
   * RawGeometry
   */

  RectF RawGeometry::compute_bounds() const
  {
    if (vertices.empty()) {
      return RectF::from_size({ 0.0f, 0.0f });
    }

    const Vec2F first = vertices.front().location;

    const Vec2F min = std::accumulate(vertices.begin(), vertices.end(), first, [](Vec2F lhs, const Vertex& rhs) {
      return gf::min(lhs, rhs.location);
    });

    const Vec2F max = std::accumulate(vertices.begin(), vertices.end(), first, [](Vec2F lhs, const Vertex& rhs) {
      return gf::max(lhs, rhs.location);
    });

    return RectF::from_min_max(min, max);
  }

  /*
   * RawSplitGeometry
   */

  void RawSplitGeometry::add_quad(uint32_t texture_index, StaticSpan<const Vertex, 4> vertices)
  {
    auto& geometry = map[texture_index];
    auto index = static_cast<uint16_t>(geometry.vertices.size());

    geometry.vertices.insert(geometry.vertices.end(), vertices.begin(), vertices.end());

    // first triangle
    geometry.indices.push_back(index);
    geometry.indices.push_back(index + 1);
    geometry.indices.push_back(index + 2);

    // second triangle
    geometry.indices.push_back(index + 2);
    geometry.indices.push_back(index + 1);
    geometry.indices.push_back(index + 3);
  }

  RawGeometry RawSplitGeometry::merge() const
  {
    RawGeometry merged_geometry;

    for (const auto& [texture_index, geometry] : map) {
      merged_geometry.merge_with(geometry);
    }

    return merged_geometry;
  }

}
