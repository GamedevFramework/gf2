// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Grids.h>
// clang-format on

#include <cmath>
#include <cstdlib>

#include <algorithm>

#include <gf2/Math.h>
#include <gf2/Span.h>

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

  OrthogonalGrid::OrthogonalGrid(Vec2I layer_size, Vec2F tile_size)
  : m_layer_size(layer_size)
  , m_tile_size(tile_size)
  {
  }

  RectF OrthogonalGrid::compute_bounds() const
  {
    return RectF::from_size(m_layer_size * m_tile_size);
  }

  RectI OrthogonalGrid::compute_visible_area(RectF local) const
  {
    return RectI::from_min_max(local.min() / m_tile_size, local.max() / m_tile_size);
  }

  RectF OrthogonalGrid::compute_cell_bounds(Vec2I coordinates) const
  {
    return RectF::from_position_size(coordinates * m_tile_size, m_tile_size);
  }

  Vec2I OrthogonalGrid::compute_coordinates(Vec2F position) const
  {
    return floorvec(position.x / m_tile_size.w, position.y / m_tile_size.h);
  }

  std::vector<Vec2F> OrthogonalGrid::compute_contour(Vec2I coordinates) const
  {
    const RectF bounds = compute_cell_bounds(coordinates);

    std::vector<Vec2F> contour;
    contour.push_back(bounds.position_at(Orientation::NorthWest));
    contour.push_back(bounds.position_at(Orientation::NorthEast));
    contour.push_back(bounds.position_at(Orientation::SouthEast));
    contour.push_back(bounds.position_at(Orientation::SouthWest));
    return contour;
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  std::vector<Vec2I> OrthogonalGrid::compute_neighbors(Vec2I coordinates, Flags<CellNeighborQuery> flags) const
  {
    std::vector<Vec2I> neighbors;

    neighbors.push_back(coordinates + gf::vec(-1, +0));
    neighbors.push_back(coordinates + gf::vec(+1, +0));
    neighbors.push_back(coordinates + gf::vec(+0, -1));
    neighbors.push_back(coordinates + gf::vec(+0, +1));

    if (flags.test(CellNeighborQuery::Diagonal)) {
      neighbors.push_back(coordinates + gf::vec(-1, -1));
      neighbors.push_back(coordinates + gf::vec(+1, -1));
      neighbors.push_back(coordinates + gf::vec(-1, +1));
      neighbors.push_back(coordinates + gf::vec(+1, +1));
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
  bool OrthogonalGrid::are_diagonal_neighbors(Vec2I coordinates0, Vec2I coordinates1) const
  {
    return std::abs(coordinates0.x - coordinates1.x) == 1 && std::abs(coordinates0.y - coordinates1.y) == 1;
  }

  /*
   * IsometricGrid
   */

  IsometricGrid::IsometricGrid(Vec2I layer_size, Vec2F tile_size)
  : m_layer_size(layer_size)
  , m_tile_size(tile_size)
  {
  }

  RectF IsometricGrid::compute_bounds() const
  {
    return RectF::from_size((m_layer_size.w + m_layer_size.h) * m_tile_size / 2.0f);
  }

  RectI IsometricGrid::compute_visible_area(RectF local) const
  {
    const Vec2I north_west = compute_coordinates(local.position_at(Orientation::NorthWest));
    const Vec2I north_east = compute_coordinates(local.position_at(Orientation::NorthEast));
    const Vec2I south_west = compute_coordinates(local.position_at(Orientation::SouthWest));
    const Vec2I south_east = compute_coordinates(local.position_at(Orientation::SouthEast));
    return RectI::from_min_max({ north_west.x, north_east.y }, { south_east.x, south_west.y });
  }

  RectF IsometricGrid::compute_cell_bounds(Vec2I coordinates) const
  {
    const Vec2I transformed = { coordinates.x - coordinates.y + m_layer_size.h - 1, coordinates.x + coordinates.y };
    return RectF::from_position_size(transformed * m_tile_size / 2.0f, m_tile_size);
  }

  Vec2I IsometricGrid::compute_coordinates(Vec2F position) const
  {
    position.x -= static_cast<float>(m_layer_size.h) * m_tile_size.w / 2.0f;
    const Vec2F transformed = position / (m_tile_size / 2.0f);
    return floorvec((transformed.x + transformed.y) / 2.0f, (transformed.y - transformed.x) / 2.0f);
  }

  std::vector<Vec2F> IsometricGrid::compute_contour(Vec2I coordinates) const
  {
    const RectF bounds = compute_cell_bounds(coordinates);

    std::vector<Vec2F> contour;
    contour.push_back(bounds.position_at(Orientation::North));
    contour.push_back(bounds.position_at(Orientation::East));
    contour.push_back(bounds.position_at(Orientation::South));
    contour.push_back(bounds.position_at(Orientation::West));
    return contour;
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  std::vector<Vec2I> IsometricGrid::compute_neighbors(Vec2I coordinates, Flags<CellNeighborQuery> flags) const
  {
    std::vector<Vec2I> neighbors;

    neighbors.push_back(coordinates + gf::vec(-1, +0));
    neighbors.push_back(coordinates + gf::vec(+1, +0));
    neighbors.push_back(coordinates + gf::vec(+0, -1));
    neighbors.push_back(coordinates + gf::vec(+0, +1));

    if (flags.test(CellNeighborQuery::Diagonal)) {
      neighbors.push_back(coordinates + gf::vec(-1, -1));
      neighbors.push_back(coordinates + gf::vec(+1, -1));
      neighbors.push_back(coordinates + gf::vec(-1, +1));
      neighbors.push_back(coordinates + gf::vec(+1, +1));
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
  bool IsometricGrid::are_diagonal_neighbors(Vec2I coordinates0, Vec2I coordinates1) const
  {
    return std::abs(coordinates0.x - coordinates1.x) == 1 && std::abs(coordinates0.y - coordinates1.y) == 1;
  }

  /*
   * StaggeredGrid
   */

  StaggeredGrid::StaggeredGrid(Vec2I layer_size, Vec2F tile_size, CellAxis axis, CellIndex index)
  : m_layer_size(layer_size)
  , m_tile_size(tile_size)
  , m_axis(axis)
  , m_index(index)
  {
  }

  RectF StaggeredGrid::compute_bounds() const
  {
    Vec2F base = m_layer_size * m_tile_size;

    switch (m_axis) {
      case CellAxis::X:
        base.x /= 2.0f;
        base.x += m_tile_size.w / 2.0f;
        base.y += m_tile_size.h / 2.0f;
        break;
      case CellAxis::Y:
        base.y /= 2.0f;
        base.y += m_tile_size.h / 2.0f;
        base.x += m_tile_size.w / 2.0f;
        break;
    }

    return RectF::from_size(base);
  }

  RectI StaggeredGrid::compute_visible_area(RectF local) const
  {
    return RectI::from_min_max(compute_coordinates(local.min()), compute_coordinates(local.max()));
  }

  RectF StaggeredGrid::compute_cell_bounds(Vec2I coordinates) const
  {
    Vec2F base = coordinates * m_tile_size;

    switch (m_axis) {
      case CellAxis::X:
        base.x /= 2.0f;

        if ((m_index == CellIndex::Even) == (coordinates.x % 2 == 0)) {
          base.y += m_tile_size.h / 2;
        }
        break;
      case CellAxis::Y:
        base.y /= 2.0f;

        if ((m_index == CellIndex::Even) == (coordinates.y % 2 == 0)) {
          base.x += m_tile_size.w / 2.0f;
        }
        break;
    }

    return RectF::from_position_size(base, m_tile_size);
  }

  Vec2I StaggeredGrid::compute_coordinates(Vec2F position) const
  {
    const Vec2F half = m_tile_size / 2.0f;

    const float qx = std::floor(position.x / half.x);
    const float rx = (position.x - qx * half.x) / half.x;
    assert(0 <= rx && rx < 1.0f);

    const float qy = std::floor(position.y / half.y);
    const float ry = (position.y - qy * half.y) / half.y;
    assert(0 <= ry && ry < 1.0f);

    const int x = static_cast<int>(qx);
    const int y = static_cast<int>(qy);

    const bool is_diagonally_split = (m_index == CellIndex::Even) == (parity(x) == parity(y));

    gf::Vec2I coordinates = vec(x, y);

    if (m_axis == CellAxis::X) {
      if ((is_diagonally_split && rx < ry) || (!is_diagonally_split && (rx + ry) < 1)) {
        --coordinates.x;
      }

      coordinates.y = div_floor(y, 2);

      if (parity(y) == 0 && ((is_diagonally_split && rx > ry) || (!is_diagonally_split && (rx + ry) < 1))) {
        --coordinates.y;
      }
    } else {
      if ((is_diagonally_split && rx > ry) || (!is_diagonally_split && (rx + ry) < 1)) {
        --coordinates.y;
      }

      coordinates.x = div_floor(x, 2);

      if (parity(x) == 0 && ((is_diagonally_split && rx < ry) || (!is_diagonally_split && (rx + ry) < 1))) {
        --coordinates.x;
      }
    }

    return coordinates;
  }

  std::vector<Vec2F> StaggeredGrid::compute_contour(Vec2I coordinates) const
  {
    const RectF bounds = compute_cell_bounds(coordinates);

    std::vector<Vec2F> contour;
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

  std::vector<Vec2I> StaggeredGrid::compute_neighbors(Vec2I coordinates, Flags<CellNeighborQuery> flags) const
  {
    StaticSpan<const Vec2I, 4> relative;
    StaticSpan<const Vec2I, 4> diagonal;

    switch (m_axis) {
      case CellAxis::X:
        if ((m_index == CellIndex::Even) == (coordinates.x % 2 == 0)) {
          relative = StaggeredNeighborsX1;
        } else {
          relative = StaggeredNeighborsX0;
        }

        diagonal = StaggeredNeighborsXD;
        break;
      case CellAxis::Y:
        if ((m_index == CellIndex::Even) == (coordinates.y % 2 == 0)) {
          relative = StaggeredNeighborsY1;
        } else {
          relative = StaggeredNeighborsY0;
        }

        diagonal = StaggeredNeighborsYD;
        break;
    }

    std::vector<Vec2I> neighbors;

    for (auto offset : relative) {
      neighbors.push_back(coordinates + offset);
    }

    assert(neighbors.size() == 4);

    if (flags.test(CellNeighborQuery::Diagonal)) {
      for (auto offset : diagonal) {
        neighbors.push_back(coordinates + offset);
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

  bool StaggeredGrid::are_diagonal_neighbors(Vec2I coordinates0, Vec2I coordinates1) const
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

    const Vec2I offset = coordinates1 - coordinates0;
    return std::any_of(diagonal.begin(), diagonal.end(), [offset](auto other) { return other == offset; });
  }

  /*
   * HexagonalGrid
   */

  namespace {

    constexpr Vec2F compute_offset(Vec2F tile_size, float side_length)
    {
      return { (tile_size.w - side_length) / 2.0f, (tile_size.h - side_length) / 2.0f };
    }

  } // namespace

  HexagonalGrid::HexagonalGrid(Vec2I layer_size, Vec2F tile_size, float side_length, CellAxis axis, CellIndex index)
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
  , m_side_length(radius)
  , m_axis(axis)
  , m_index(index)
  {
  }

  RectF HexagonalGrid::compute_bounds() const
  {
    Vec2F size = gf::vec(0.0f, 0.0f);
    const Vec2F offset = compute_offset(m_tile_size, m_side_length);

    switch (m_axis) {
      case CellAxis::X:
        size.w = static_cast<float>(m_layer_size.w) * (m_tile_size.w - offset.w) + offset.w;
        size.h = static_cast<float>(m_layer_size.h) * m_tile_size.h + m_tile_size.h / 2.0f;
        break;

      case CellAxis::Y:
        size.w = static_cast<float>(m_layer_size.w) * m_tile_size.w + m_tile_size.w / 2.0f;
        size.h = static_cast<float>(m_layer_size.h) * (m_tile_size.h - offset.h) + offset.h;
        break;
    }

    return RectF::from_size(size);
  }

  RectI HexagonalGrid::compute_visible_area(RectF local) const
  {
    return RectI::from_min_max(compute_coordinates(local.min()), compute_coordinates(local.max()));
  }

  RectF HexagonalGrid::compute_cell_bounds(Vec2I coordinates) const
  {
    Vec2F base = gf::vec(0.0f, 0.0f);
    const Vec2F offset = compute_offset(m_tile_size, m_side_length);

    switch (m_axis) {
      case CellAxis::X:
        base.x = static_cast<float>(coordinates.x) * (m_tile_size.w - offset.w);
        base.y = static_cast<float>(coordinates.y) * m_tile_size.h;

        if ((m_index == CellIndex::Even) == (coordinates.x % 2 == 0)) {
          base.y += m_tile_size.h / 2.0f;
        }
        break;

      case CellAxis::Y:
        base.x = static_cast<float>(coordinates.x) * m_tile_size.w;
        base.y = static_cast<float>(coordinates.y) * (m_tile_size.h - offset.h);

        if ((m_index == CellIndex::Even) == (coordinates.y % 2 == 0)) {
          base.x += m_tile_size.w / 2.0f;
        }
        break;
    }

    return RectF::from_position_size(base, m_tile_size);
  }

  Vec2I HexagonalGrid::compute_coordinates_x_axis(Vec2F position) const
  {
    assert(m_axis == CellAxis::X);
    const Vec2F offset = compute_offset(m_tile_size, m_side_length);

    const float tx = m_tile_size.w - offset.w;
    const float qx = std::floor(position.x / tx);
    const float rx = position.x - qx * tx;
    const float nrx = rx / offset.w;

    const float ty = m_tile_size.h / 2.0f;
    const float qy = std::floor(position.y / ty);
    const float ry = position.y - qy * ty;
    const float nry = ry / ty;

    const int x = static_cast<int>(qx);
    const int y = static_cast<int>(qy);

    Vec2I coordinates = vec(x, y);

    if ((m_index == CellIndex::Even) == (parity(x) == 0)) {
      --coordinates.y;
    }

    coordinates.y = div_floor(coordinates.y, 2);

    if (rx >= offset.w) {
      return coordinates;
    }

    assert(0 <= nrx && nrx < 1);
    assert(0 <= nry && nry < 1);

    if ((m_index == CellIndex::Even) == (parity(x) == 0)) {
      if (parity(y) == 0) {
        if (nrx < nry) {
          --coordinates.x;
          ++coordinates.y;
        }
      } else {
        if (nrx + nry < 1) {
          --coordinates.x;
        }
      }
    } else {
      if (parity(y) == 0) {
        if (nrx + nry < 1) {
          --coordinates.x;
          --coordinates.y;
        }
      } else {
        if (nrx < nry) {
          --coordinates.x;
        }
      }
    }

    return coordinates;
  }

  Vec2I HexagonalGrid::compute_coordinates_y_axis(Vec2F position) const
  {
    assert(m_axis == CellAxis::Y);
    const Vec2F offset = compute_offset(m_tile_size, m_side_length);

    const float ty = m_tile_size.h - offset.h;
    const float qy = std::floor(position.y / ty);
    const float ry = position.y - qy * ty;
    const float nry = ty / offset.h;

    const float tx = m_tile_size.w / 2.0f;
    const float qx = std::floor(position.x / tx);
    const float rx = position.x - qx * tx;
    const float nrx = rx / tx;

    const int x = static_cast<int>(qx);
    const int y = static_cast<int>(qy);

    Vec2I coordinates = vec(x, y);

    if ((m_index == CellIndex::Even) == (parity(y) == 0)) {
      --coordinates.x;
    }

    coordinates.x = div_floor(coordinates.x, 2);

    if (ry >= offset.h) {
      return coordinates;
    }

    assert(0 <= nrx && nrx < 1);
    assert(0 <= nry && nry < 1);

    if ((m_index == CellIndex::Even) == (parity(y) == 0)) {
      if (parity(x) == 0) {
        if (nrx > nry) {
          --coordinates.y;
          ++coordinates.x;
        }
      } else {
        if (nrx + nry < 1) {
          --coordinates.y;
        }
      }
    } else {
      if (parity(x) == 0) {
        if (nrx + nry < 1) {
          --coordinates.y;
          --coordinates.x;
        }
      } else {
        if (nrx > nry) {
          --coordinates.y;
        }
      }
    }

    return coordinates;
  }

  Vec2I HexagonalGrid::compute_coordinates(Vec2F position) const
  {
    switch (m_axis) {
      case CellAxis::X:
        return compute_coordinates_x_axis(position);

      case CellAxis::Y:
        return compute_coordinates_y_axis(position);
    }

    return {};
  }

  std::vector<Vec2F> HexagonalGrid::compute_contour(Vec2I coordinates) const
  {
    const Vec2F offset = compute_offset(m_tile_size, m_side_length);
    const RectF bounds = compute_cell_bounds(coordinates);
    const Vec2F min = bounds.min();
    const Vec2F max = bounds.max();

    std::vector<Vec2F> contour;

    switch (m_axis) {
      case CellAxis::X:
        // clang-format off
        contour.emplace_back(min.x,             (min.y + max.y) / 2.0f);
        contour.emplace_back(min.x + offset.w,  min.y);
        contour.emplace_back(max.x - offset.w,  min.y);
        contour.emplace_back(max.x,             (min.y + max.y) / 2.0f);
        contour.emplace_back(max.x - offset.w,  max.y);
        contour.emplace_back(min.x + offset.w,  max.y);
        // clang-format on
        break;

      case CellAxis::Y:
        // clang-format off
        contour.emplace_back((min.x + max.x) / 2.0f, min.y);
        contour.emplace_back(min.x,                  min.y + offset.h);
        contour.emplace_back(min.x,                  max.y - offset.h);
        contour.emplace_back((min.x + max.x) / 2.0f, max.y);
        contour.emplace_back(max.x,                  max.y - offset.h);
        contour.emplace_back(max.x,                  min.y + offset.h);
        // clang-format on
        break;
    }

    return contour;
  }

  std::vector<Vec2I> HexagonalGrid::compute_neighbors(Vec2I coordinates, Flags<CellNeighborQuery> flags) const
  {
    static constexpr Vec2I XOffsets[2][6] = {
      {{ +1, +0 }, { +1, -1 }, { +0, -1 }, { -1, -1 }, { -1, +0 }, { +0, +1 }},
      {{ +1, +1 }, { +1, +0 }, { +0, -1 }, { -1, +0 }, { -1, +1 }, { +0, +1 }}
    };

    static constexpr Vec2I YOffsets[2][6] = {
      {{ +1, +0 }, { +0, -1 }, { -1, -1 }, { -1, +0 }, { -1, +1 }, { +0, +1 }},
      {{ +1, +0 }, { +1, -1 }, { +0, -1 }, { -1, +0 }, { +0, +1 }, { +1, +1 }}
    };

    StaticSpan<const Vec2I, 6> relative;

    switch (m_axis) {
      case CellAxis::X:
        if ((m_index == CellIndex::Even) == (coordinates.x % 2 == 0)) {
          relative = XOffsets[1];
        } else {
          relative = XOffsets[0];
        }
        break;
      case CellAxis::Y:
        if ((m_index == CellIndex::Even) == (coordinates.y % 2 == 0)) {
          relative = YOffsets[1];
        } else {
          relative = YOffsets[0];
        }
        break;
    }

    std::vector<Vec2I> neighbors;

    for (auto offset : relative) {
      neighbors.push_back(coordinates + offset);
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
  bool HexagonalGrid::are_diagonal_neighbors([[maybe_unused]] Vec2I coordinates0, [[maybe_unused]] Vec2I coordinates1) const
  {
    return false; // there are no diagonal neighbors in hexagonal grids
  }

  Vec2F HexagonalGrid::compute_regular_size(CellAxis axis, float radius)
  {
    Vec2F size = gf::vec(0.0f, 0.0f);

    switch (axis) {
      case CellAxis::X:
        size = { radius * 2.0f, radius * Sqrt3 };
        break;

      case CellAxis::Y:
        size = { radius * Sqrt3, radius * 2.0f };
        break;
    }

    return size;
  }

} // namespace gf