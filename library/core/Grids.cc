// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/Grids.h>

#include <cmath>
#include <cstdlib>

#include <algorithm>

#include <gf2/core/Math.h>
#include <gf2/core/Span.h>

namespace gf {

  namespace {
    int floorint(float x)
    {
      return static_cast<int>(std::floor(x));
    }

    Vec2I floorvec(float x, float y)
    {
      return { floorint(x), floorint(y) };
    }

  }

  /*
   * OrthogonalGrid
   */

  OrthogonalGrid::OrthogonalGrid(Vec2I layer_size, Vec2I tile_size)
  : m_layer_size(layer_size)
  , m_tile_size(tile_size)
  {
  }

  RectI OrthogonalGrid::compute_bounds() const
  {
    return RectI::from_size(m_layer_size * m_tile_size);
  }

  RectI OrthogonalGrid::compute_visible_area(RectF local) const
  {
    return RectI::from_min_max(local.min() / m_tile_size, local.max() / m_tile_size);
  }

  RectI OrthogonalGrid::compute_cell_bounds(Vec2I position) const
  {
    return RectI::from_position_size(position * m_tile_size, m_tile_size);
  }

  Vec2I OrthogonalGrid::compute_position(Vec2F location) const
  {
    location /= m_tile_size;
    return floorvec(location.x, location.y);
  }

  std::vector<Vec2I> OrthogonalGrid::compute_contour(Vec2I position) const
  {
    const RectI bounds = compute_cell_bounds(position);

    std::vector<Vec2I> contour;
    contour.push_back(bounds.position_at(Orientation::NorthWest));
    contour.push_back(bounds.position_at(Orientation::NorthEast));
    contour.push_back(bounds.position_at(Orientation::SouthEast));
    contour.push_back(bounds.position_at(Orientation::SouthWest));
    return contour;
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  std::vector<Vec2I> OrthogonalGrid::compute_neighbors(Vec2I position, Flags<CellNeighborQuery> flags) const
  {
    std::vector<Vec2I> neighbors;

    neighbors.push_back(position + gf::vec(-1, +0));
    neighbors.push_back(position + gf::vec(+1, +0));
    neighbors.push_back(position + gf::vec(+0, -1));
    neighbors.push_back(position + gf::vec(+0, +1));

    if (flags.test(CellNeighborQuery::Diagonal)) {
      neighbors.push_back(position + gf::vec(-1, -1));
      neighbors.push_back(position + gf::vec(+1, -1));
      neighbors.push_back(position + gf::vec(-1, +1));
      neighbors.push_back(position + gf::vec(+1, +1));
    }

    if (flags.test(CellNeighborQuery::Valid)) {
      const RectI bounds = RectI::from_size(m_layer_size);
      // clang-format off
      neighbors.erase(std::remove_if(neighbors.begin(), neighbors.end(), [bounds](Vec2I neighbor) {
        return !bounds.contains(neighbor);
      }), neighbors.end());
      // clang-format on
    }

    return neighbors;
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  bool OrthogonalGrid::are_diagonal_neighbors(Vec2I position0, Vec2I position1) const
  {
    return std::abs(position0.x - position1.x) == 1 && std::abs(position0.y - position1.y) == 1;
  }

  /*
   * IsometricGrid
   */

  IsometricGrid::IsometricGrid(Vec2I layer_size, Vec2I tile_size)
  : m_layer_size(layer_size)
  , m_tile_size(tile_size)
  {
  }

  RectI IsometricGrid::compute_bounds() const
  {
    return RectI::from_size((m_layer_size.w + m_layer_size.h) * m_tile_size / 2);
  }

  RectI IsometricGrid::compute_visible_area(RectF local) const
  {
    const Vec2I north_west = compute_position(local.position_at(Orientation::NorthWest));
    const Vec2I north_east = compute_position(local.position_at(Orientation::NorthEast));
    const Vec2I south_west = compute_position(local.position_at(Orientation::SouthWest));
    const Vec2I south_east = compute_position(local.position_at(Orientation::SouthEast));
    return RectI::from_min_max({ north_west.x, north_east.y }, { south_east.x, south_west.y });
  }

  RectI IsometricGrid::compute_cell_bounds(Vec2I position) const
  {
    const Vec2I transformed = { position.x - position.y + m_layer_size.h - 1, position.x + position.y };
    return RectI::from_position_size(transformed * m_tile_size / 2, m_tile_size);
  }

  Vec2I IsometricGrid::compute_position(Vec2F location) const
  {
    location.x -= static_cast<float>(m_layer_size.h * m_tile_size.w) / 2.0f;
    const Vec2F transformed = location / (m_tile_size / 2.0f);
    return floorvec((transformed.x + transformed.y) / 2.0f, (transformed.y - transformed.x) / 2.0f);
  }

  std::vector<Vec2I> IsometricGrid::compute_contour(Vec2I position) const
  {
    const RectI bounds = compute_cell_bounds(position);

    std::vector<Vec2I> contour;
    contour.push_back(bounds.position_at(Orientation::North));
    contour.push_back(bounds.position_at(Orientation::East));
    contour.push_back(bounds.position_at(Orientation::South));
    contour.push_back(bounds.position_at(Orientation::West));
    return contour;
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  std::vector<Vec2I> IsometricGrid::compute_neighbors(Vec2I position, Flags<CellNeighborQuery> flags) const
  {
    std::vector<Vec2I> neighbors;

    neighbors.push_back(position + gf::vec(-1, +0));
    neighbors.push_back(position + gf::vec(+1, +0));
    neighbors.push_back(position + gf::vec(+0, -1));
    neighbors.push_back(position + gf::vec(+0, +1));

    if (flags.test(CellNeighborQuery::Diagonal)) {
      neighbors.push_back(position + gf::vec(-1, -1));
      neighbors.push_back(position + gf::vec(+1, -1));
      neighbors.push_back(position + gf::vec(-1, +1));
      neighbors.push_back(position + gf::vec(+1, +1));
    }

    if (flags.test(CellNeighborQuery::Valid)) {
      const RectI bounds = RectI::from_size(m_layer_size);
      // clang-format off
      neighbors.erase(std::remove_if(neighbors.begin(), neighbors.end(), [bounds](Vec2I neighbor) {
        return !bounds.contains(neighbor);
      }), neighbors.end());
      // clang-format on
    }

    return neighbors;
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  bool IsometricGrid::are_diagonal_neighbors(Vec2I position0, Vec2I position1) const
  {
    return std::abs(position0.x - position1.x) == 1 && std::abs(position0.y - position1.y) == 1;
  }

  /*
   * StaggeredGrid
   */

  StaggeredGrid::StaggeredGrid(Vec2I layer_size, Vec2I tile_size, CellAxis axis, CellIndex index)
  : m_layer_size(layer_size)
  , m_tile_size(tile_size)
  , m_axis(axis)
  , m_index(index)
  {
  }

  RectI StaggeredGrid::compute_bounds() const
  {
    Vec2I base = m_layer_size * m_tile_size;

    switch (m_axis) {
      case CellAxis::X:
        base.x /= 2;
        base.x += m_tile_size.w / 2;
        base.y += m_tile_size.h / 2;
        break;
      case CellAxis::Y:
        base.y /= 2;
        base.y += m_tile_size.h / 2;
        base.x += m_tile_size.w / 2;
        break;
    }

    return RectI::from_size(base);
  }

  RectI StaggeredGrid::compute_visible_area(RectF local) const
  {
    return RectI::from_min_max(compute_position(local.min()), compute_position(local.max()));
  }

  RectI StaggeredGrid::compute_cell_bounds(Vec2I position) const
  {
    Vec2I base = position * m_tile_size;

    switch (m_axis) {
      case CellAxis::X:
        base.x /= 2;

        if ((m_index == CellIndex::Even) == (position.x % 2 == 0)) {
          base.y += m_tile_size.h / 2;
        }
        break;
      case CellAxis::Y:
        base.y /= 2;

        if ((m_index == CellIndex::Even) == (position.y % 2 == 0)) {
          base.x += m_tile_size.w / 2;
        }
        break;
    }

    return RectI::from_position_size(base, m_tile_size);
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  Vec2I StaggeredGrid::compute_position_x_axis(bool is_diagonally_split, Vec2I q, Vec2F r) const
  {
    Vec2I position = q;

    if ((is_diagonally_split && r.x < r.y) || (!is_diagonally_split && (r.x + r.y) < 1)) {
      --position.x;
    }

    position.y = div_floor(q.y, 2);

    if (parity(q.y) == 0 && ((is_diagonally_split && r.x > r.y) || (!is_diagonally_split && (r.x + r.y) < 1))) {
      --position.y;
    }

    return position;
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  Vec2I StaggeredGrid::compute_position_y_axis(bool is_diagonally_split, Vec2I q, Vec2F r) const
  {
    Vec2I position = q;

    if ((is_diagonally_split && r.x > r.y) || (!is_diagonally_split && (r.x + r.y) < 1)) {
      --position.y;
    }

    position.x = div_floor(q.x, 2);

    if (parity(q.x) == 0 && ((is_diagonally_split && r.x < r.y) || (!is_diagonally_split && (r.x + r.y) < 1))) {
      --position.x;
    }

    return position;
  }

  Vec2I StaggeredGrid::compute_position(Vec2F location) const
  {
    const Vec2F half = m_tile_size / 2.0f;

    const float qx = std::floor(location.x / half.x);
    const float rx = (location.x - qx * half.x) / half.x;
    assert(0 <= rx && rx < 1.0f);

    const float qy = std::floor(location.y / half.y);
    const float ry = (location.y - qy * half.y) / half.y;
    assert(0 <= ry && ry < 1.0f);

    const int x = static_cast<int>(qx);
    const int y = static_cast<int>(qy);

    const bool is_diagonally_split = (m_index == CellIndex::Even) == (parity(x) == parity(y));

    Vec2I position = { x, y };

    switch (m_axis) {
      case CellAxis::X:
        position = compute_position_x_axis(is_diagonally_split, position, { rx, ry });
        break;
      case CellAxis::Y:
        position = compute_position_y_axis(is_diagonally_split, position, { rx, ry });
        break;
    }

    return position;
  }

  std::vector<Vec2I> StaggeredGrid::compute_contour(Vec2I position) const
  {
    const RectI bounds = compute_cell_bounds(position);

    std::vector<Vec2I> contour;
    contour.push_back(bounds.position_at(Orientation::North));
    contour.push_back(bounds.position_at(Orientation::East));
    contour.push_back(bounds.position_at(Orientation::South));
    contour.push_back(bounds.position_at(Orientation::West));
    return contour;
  }

  namespace {
    // clang-format off
    constexpr Vec2I StaggeredNeighborsX1[4] = {{ -1, +0 }, { +1, +0 }, { -1, +1 }, { +1, +1 }};
    constexpr Vec2I StaggeredNeighborsX0[4] = {{ -1, -1 }, { +1, -1 }, { -1, +0 }, { +1, +0 }};
    constexpr Vec2I StaggeredNeighborsXD[4] = {{ -2, +0 }, { +0, -1 }, { +2, +0 }, { +0, +1 }};

    constexpr Vec2I StaggeredNeighborsY1[4] = {{ +0, -1 }, { +1, -1 }, { +0, +1 }, { +1, +1 }};
    constexpr Vec2I StaggeredNeighborsY0[4] = {{ -1, -1 }, { +0, -1 }, { -1, +1 }, { +0, +1 }};
    constexpr Vec2I StaggeredNeighborsYD[4] = {{ -2, +0 }, { +0, -1 }, { +2, +0 }, { +0, +1 }};
    // clang-format on
  }

  std::vector<Vec2I> StaggeredGrid::compute_neighbors(Vec2I position, Flags<CellNeighborQuery> flags) const
  {
    StaticSpan<const Vec2I, 4> relative;
    StaticSpan<const Vec2I, 4> diagonal;

    switch (m_axis) {
      case CellAxis::X:
        if ((m_index == CellIndex::Even) == (position.x % 2 == 0)) {
          relative = StaggeredNeighborsX1;
        } else {
          relative = StaggeredNeighborsX0;
        }

        diagonal = StaggeredNeighborsXD;
        break;
      case CellAxis::Y:
        if ((m_index == CellIndex::Even) == (position.y % 2 == 0)) {
          relative = StaggeredNeighborsY1;
        } else {
          relative = StaggeredNeighborsY0;
        }

        diagonal = StaggeredNeighborsYD;
        break;
    }

    std::vector<Vec2I> neighbors;

    for (auto offset : relative) {
      neighbors.push_back(position + offset);
    }

    assert(neighbors.size() == 4);

    if (flags.test(CellNeighborQuery::Diagonal)) {
      for (auto offset : diagonal) {
        neighbors.push_back(position + offset);
      }
    }

    if (flags.test(CellNeighborQuery::Valid)) {
      const RectI bounds = RectI::from_size(m_layer_size);
      // clang-format off
      neighbors.erase(std::remove_if(neighbors.begin(), neighbors.end(), [bounds](Vec2I neighbor) {
        return !bounds.contains(neighbor);
      }), neighbors.end());
      // clang-format on
    }

    return neighbors;
  }

  bool StaggeredGrid::are_diagonal_neighbors(Vec2I position0, Vec2I position1) const
  {
    StaticSpan<const Vec2I, 4> diagonal;

    switch (m_axis) {
      case CellAxis::X:
        diagonal = StaggeredNeighborsXD;
        break;
      case CellAxis::Y:
        diagonal = StaggeredNeighborsYD;
        break;
    }

    const Vec2I offset = position1 - position0;
    return std::any_of(diagonal.begin(), diagonal.end(), [offset](auto other) { return other == offset; });
  }

  /*
   * HexagonalGrid
   */

  namespace {

    constexpr Vec2I compute_offset(Vec2I tile_size, int32_t side_length)
    {
      return { (tile_size.w - side_length) / 2, (tile_size.h - side_length) / 2 };
    }

  } // namespace

  HexagonalGrid::HexagonalGrid(Vec2I layer_size, Vec2I tile_size, int32_t side_length, CellAxis axis, CellIndex index)
  : m_layer_size(layer_size)
  , m_tile_size(tile_size)
  , m_side_length(side_length)
  , m_axis(axis)
  , m_index(index)
  {
  }

  HexagonalGrid::HexagonalGrid(Vec2I layer_size, float radius, CellAxis axis, CellIndex index)
  : m_layer_size(layer_size)
  , m_tile_size(compute_regular_size(axis, radius))
  , m_side_length(static_cast<int32_t>(radius))
  , m_axis(axis)
  , m_index(index)
  {
  }

  RectI HexagonalGrid::compute_bounds() const
  {
    Vec2I size = { 0, 0 };
    const Vec2I offset = compute_offset(m_tile_size, m_side_length);

    switch (m_axis) {
      case CellAxis::X:
        size.w = m_layer_size.w * (m_tile_size.w - offset.w) + offset.w;
        size.h = m_layer_size.h * m_tile_size.h + m_tile_size.h / 2;
        break;

      case CellAxis::Y:
        size.w = m_layer_size.w * m_tile_size.w + m_tile_size.w / 2;
        size.h = m_layer_size.h * (m_tile_size.h - offset.h) + offset.h;
        break;
    }

    return RectI::from_size(size);
  }

  RectI HexagonalGrid::compute_visible_area(RectF local) const
  {
    return RectI::from_min_max(compute_position(local.min()), compute_position(local.max()));
  }

  RectI HexagonalGrid::compute_cell_bounds(Vec2I position) const
  {
    Vec2I base = { 0, 0 };
    const Vec2I offset = compute_offset(m_tile_size, m_side_length);

    switch (m_axis) {
      case CellAxis::X:
        base.x = position.x * (m_tile_size.w - offset.w);
        base.y = position.y * m_tile_size.h;

        if ((m_index == CellIndex::Even) == (position.x % 2 == 0)) {
          base.y += m_tile_size.h / 2;
        }
        break;

      case CellAxis::Y:
        base.x = position.x * m_tile_size.w;
        base.y = position.y * (m_tile_size.h - offset.h);

        if ((m_index == CellIndex::Even) == (position.y % 2 == 0)) {
          base.x += m_tile_size.w / 2;
        }
        break;
    }

    return RectI::from_position_size(base, m_tile_size);
  }

  namespace {

    Vec2I compute_x_when_identical_parity(Vec2I position, Vec2I q, Vec2F r)
    {
      if (parity(q.y) == 0) {
        if (r.x < r.y) {
          --position.x;
          ++position.y;
        }
      } else {
        if (r.x + r.y < 1) {
          --position.x;
        }
      }

      return position;
    }

    Vec2I compute_x_when_different_parity(Vec2I position, Vec2I q, Vec2F r)
    {
      if (parity(q.y) == 0) {
        if (r.x + r.y < 1) {
          --position.x;
          --position.y;
        }
      } else {
        if (r.x < r.y) {
          --position.x;
        }
      }

      return position;
    }
  }

  Vec2I HexagonalGrid::compute_position_x_axis(Vec2F location) const
  {
    assert(m_axis == CellAxis::X);
    const Vec2F offset = compute_offset(m_tile_size, m_side_length);

    const float tx = static_cast<float>(m_tile_size.w) - offset.w;
    const float qx = std::floor(location.x / tx);
    const float rx = location.x - qx * tx;
    const float nrx = rx / offset.w;

    const float ty = static_cast<float>(m_tile_size.h) / 2.0f;
    const float qy = std::floor(location.y / ty);
    const float ry = location.y - qy * ty;
    const float nry = ry / ty;

    const int x = static_cast<int>(qx);
    const int y = static_cast<int>(qy);

    Vec2I position = vec(x, y);

    if ((m_index == CellIndex::Even) == (parity(x) == 0)) {
      --position.y;
    }

    position.y = div_floor(position.y, 2);

    if (rx >= offset.w) {
      return position;
    }

    assert(0 <= nrx && nrx < 1);
    assert(0 <= nry && nry < 1);

    if ((m_index == CellIndex::Even) == (parity(x) == 0)) {
      position = compute_x_when_identical_parity(position, { x, y }, { nrx, nry });
    } else {
      position = compute_x_when_different_parity(position, { x, y }, { nrx, nry });
    }

    return position;
  }

  namespace {

    Vec2I compute_y_when_identical_parity(Vec2I position, Vec2I q, Vec2F r)
    {
      if (parity(q.x) == 0) {
        if (r.x > r.y) {
          --position.y;
          ++position.x;
        }
      } else {
        if (r.x + r.y < 1) {
          --position.y;
        }
      }

      return position;
    }

    Vec2I compute_y_when_different_parity(Vec2I position, Vec2I q, Vec2F r)
    {
      if (parity(q.x) == 0) {
        if (r.x + r.y < 1) {
          --position.y;
          --position.x;
        }
      } else {
        if (r.x > r.y) {
          --position.y;
        }
      }

      return position;
    }

  }

  Vec2I HexagonalGrid::compute_position_y_axis(Vec2F location) const
  {
    assert(m_axis == CellAxis::Y);
    const Vec2F offset = compute_offset(m_tile_size, m_side_length);

    const float ty = static_cast<float>(m_tile_size.h) - offset.h;
    const float qy = std::floor(location.y / ty);
    const float ry = location.y - qy * ty;
    const float nry = ry / offset.h;

    const float tx = static_cast<float>(m_tile_size.w) / 2.0f;
    const float qx = std::floor(location.x / tx);
    const float rx = location.x - qx * tx;
    const float nrx = rx / tx;

    const int x = static_cast<int>(qx);
    const int y = static_cast<int>(qy);

    Vec2I position = vec(x, y);

    if ((m_index == CellIndex::Even) == (parity(y) == 0)) {
      --position.x;
    }

    position.x = div_floor(position.x, 2);

    if (ry >= offset.h) {
      return position;
    }

    assert(0 <= nrx && nrx < 1);
    assert(0 <= nry && nry < 1);

    if ((m_index == CellIndex::Even) == (parity(y) == 0)) {
      position = compute_y_when_identical_parity(position, { x, y }, { nrx, nry });
    } else {
      position = compute_y_when_different_parity(position, { x, y }, { nrx, nry });
    }

    return position;
  }

  Vec2I HexagonalGrid::compute_position(Vec2F location) const
  {
    switch (m_axis) {
      case CellAxis::X:
        return compute_position_x_axis(location);

      case CellAxis::Y:
        return compute_position_y_axis(location);
    }

    return {};
  }

  std::vector<Vec2I> HexagonalGrid::compute_contour(Vec2I position) const
  {
    const Vec2I offset = compute_offset(m_tile_size, m_side_length);
    const RectI bounds = compute_cell_bounds(position);
    const Vec2I min = bounds.min();
    const Vec2I max = bounds.max();

    std::vector<Vec2I> contour;

    switch (m_axis) {
      case CellAxis::X:
        // clang-format off
        contour.emplace_back(min.x,             (min.y + max.y) / 2);
        contour.emplace_back(min.x + offset.w,  min.y);
        contour.emplace_back(max.x - offset.w,  min.y);
        contour.emplace_back(max.x,             (min.y + max.y) / 2);
        contour.emplace_back(max.x - offset.w,  max.y);
        contour.emplace_back(min.x + offset.w,  max.y);
        // clang-format on
        break;

      case CellAxis::Y:
        // clang-format off
        contour.emplace_back((min.x + max.x) / 2, min.y);
        contour.emplace_back(min.x,               min.y + offset.h);
        contour.emplace_back(min.x,               max.y - offset.h);
        contour.emplace_back((min.x + max.x) / 2, max.y);
        contour.emplace_back(max.x,               max.y - offset.h);
        contour.emplace_back(max.x,               min.y + offset.h);
        // clang-format on
        break;
    }

    return contour;
  }

  std::vector<Vec2I> HexagonalGrid::compute_neighbors(Vec2I position, Flags<CellNeighborQuery> flags) const
  {
    static constexpr Vec2I XOffsets[2][6] = {
      { { +1, +0 }, { +1, -1 }, { +0, -1 }, { -1, -1 }, { -1, +0 }, { +0, +1 } },
      { { +1, +1 }, { +1, +0 }, { +0, -1 }, { -1, +0 }, { -1, +1 }, { +0, +1 } }
    };

    static constexpr Vec2I YOffsets[2][6] = {
      { { +1, +0 }, { +0, -1 }, { -1, -1 }, { -1, +0 }, { -1, +1 }, { +0, +1 } },
      { { +1, +0 }, { +1, -1 }, { +0, -1 }, { -1, +0 }, { +0, +1 }, { +1, +1 } }
    };

    StaticSpan<const Vec2I, 6> relative;

    switch (m_axis) {
      case CellAxis::X:
        if ((m_index == CellIndex::Even) == (position.x % 2 == 0)) {
          relative = XOffsets[1];
        } else {
          relative = XOffsets[0];
        }
        break;
      case CellAxis::Y:
        if ((m_index == CellIndex::Even) == (position.y % 2 == 0)) {
          relative = YOffsets[1];
        } else {
          relative = YOffsets[0];
        }
        break;
    }

    std::vector<Vec2I> neighbors;

    for (auto offset : relative) {
      neighbors.push_back(position + offset);
    }

    assert(neighbors.size() == 6);

    if (flags.test(CellNeighborQuery::Valid)) {
      const RectI bounds = RectI::from_size(m_layer_size);
      // clang-format off
      neighbors.erase(std::remove_if(neighbors.begin(), neighbors.end(), [bounds](Vec2I neighbor) {
        return !bounds.contains(neighbor);
      }), neighbors.end());
      // clang-format on
    }

    return neighbors;
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  bool HexagonalGrid::are_diagonal_neighbors([[maybe_unused]] Vec2I position0, [[maybe_unused]] Vec2I position1) const
  {
    return false; // there are no diagonal neighbors in hexagonal grids
  }

  Vec2I HexagonalGrid::compute_regular_size(CellAxis axis, float radius)
  {
    Vec2F size = { 0.0f, 0.0f };

    switch (axis) {
      case CellAxis::X:
        size = { radius * 2.0f, radius * Sqrt3 };
        break;

      case CellAxis::Y:
        size = { radius * Sqrt3, radius * 2.0f };
        break;
    }

    // conversion to integers happens here
    return size;
  }

} // namespace gf
