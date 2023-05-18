// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Cells.h>
// clang-format on

#include <algorithm>

#include <gf2/Span.h>

namespace gf {

  /*
   * OrthogonalCells
   */

  OrthogonalCells::OrthogonalCells(Vec2F tile_size)
  : m_tile_size(tile_size)
  {
  }

  RectF OrthogonalCells::compute_bounds(Vec2I layer_size) const
  {
    return RectF::from_size(layer_size * m_tile_size);
  }

  RectI OrthogonalCells::compute_visible_area(RectF local) const
  {
    return RectI::from_min_max(local.min() / m_tile_size, local.max() / m_tile_size);
  }

  RectF OrthogonalCells::compute_cell_bounds(Vec2I coordinates) const
  {
    return RectF::from_position_size(coordinates * m_tile_size, m_tile_size);
  }

  Vec2I OrthogonalCells::compute_coordinates(Vec2F position) const
  {
    return position / m_tile_size;
  }

  std::vector<Vec2F> OrthogonalCells::compute_contour(Vec2I coordinates) const
  {
    const RectF bounds = compute_cell_bounds(coordinates);

    std::vector<Vec2F> contour;
    contour.push_back(bounds.position_from_anchor(Anchor::TopLeft));
    contour.push_back(bounds.position_from_anchor(Anchor::TopRight));
    contour.push_back(bounds.position_from_anchor(Anchor::BottomRight));
    contour.push_back(bounds.position_from_anchor(Anchor::BottomLeft));
    return contour;
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  std::vector<Vec2I> OrthogonalCells::compute_neighbors(Vec2I coordinates, Vec2I layer_size, Flags<CellNeighborQuery> flags) const
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
      const RectI bounds = RectI::from_size(layer_size);
      // clang-format off
      neighbors.erase(std::remove_if(neighbors.begin(), neighbors.end(), [bounds](Vec2I neighbor) {
        return !bounds.contains(neighbor);
      }), neighbors.end());
      // clang-format on
    }

    return neighbors;
  }

  /*
   * StaggeredCells
   */

  StaggeredCells::StaggeredCells(Vec2F tile_size, CellAxis axis, CellIndex index)
  : m_tile_size(tile_size)
  , m_axis(axis)
  , m_index(index)
  {
  }

  RectF StaggeredCells::compute_bounds(Vec2I layer_size) const
  {
    Vec2F base = layer_size * m_tile_size;

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

    return RectF::from_size(base);
  }

  RectI StaggeredCells::compute_visible_area(RectF local) const
  {
    return RectI::from_min_max(compute_coordinates(local.min()), compute_coordinates(local.max()));
  }

  RectF StaggeredCells::compute_cell_bounds(Vec2I coordinates) const
  {
    Vec2F base = coordinates * m_tile_size;

    switch (m_axis) {
      case CellAxis::Y:
        base.y /= 2;

        switch (m_index) {
          case CellIndex::Odd:
            if (coordinates.y % 2 != 0) {
              base.x += m_tile_size.w / 2;
            }
            break;
          case CellIndex::Even:
            if (coordinates.y % 2 == 0) {
              base.x += m_tile_size.w / 2;
            }
            break;
        }
        break;
      case CellAxis::X:
        base.x /= 2;

        switch (m_index) {
          case CellIndex::Odd:
            if (coordinates.x % 2 != 0) {
              base.y += m_tile_size.h / 2;
            }
            break;
          case CellIndex::Even:
            if (coordinates.x % 2 == 0) {
              base.y += m_tile_size.h / 2;
            }
            break;
        }
        break;
    }

    return RectF::from_position_size(base, m_tile_size);
  }

  Vec2I StaggeredCells::compute_coordinates(Vec2F position) const
  {
    // TODO: quick approximation but not really good
    auto tile_size = m_tile_size;

    switch (m_axis) {
      case CellAxis::Y:
        tile_size.y /= 2;
        break;
      case CellAxis::X:
        tile_size.x /= 2;
        break;
    }

    return position / tile_size;
  }

  std::vector<Vec2F> StaggeredCells::compute_contour(Vec2I coordinates) const
  {
    const RectF bounds = compute_cell_bounds(coordinates);
    const Vec2F min = bounds.min();
    const Vec2F max = bounds.max();

    std::vector<Vec2F> contour;
    // clang-format off
    contour.emplace_back((min.x + max.x) / 2, min.y);
    contour.emplace_back(max.x,               (min.y + max.y) / 2);
    contour.emplace_back((min.x + max.x) / 2, max.y);
    contour.emplace_back(min.x,               (min.y + max.y) / 2);
    // clang-format on

    return contour;
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  std::vector<Vec2I> StaggeredCells::compute_neighbors([[maybe_unused]] Vec2I coordinates, [[maybe_unused]] Vec2I layer_size, [[maybe_unused]] Flags<CellNeighborQuery> flags) const
  {
    // TODO
    return {};
  }

  /*
   * HexagonalCells
   */

  namespace {

    constexpr float compute_offset(Vec2F tile_size, float side_length, CellAxis axis)
    {
      switch (axis) {
        case CellAxis::X:
          return (tile_size.w - side_length) / 2;
        case CellAxis::Y:
          return (tile_size.h - side_length) / 2;
      }

      assert(false);
      return 0.0f;
    }

  } // namespace

  HexagonalCells::HexagonalCells(Vec2F tile_size, float side_length, CellAxis axis, CellIndex index)
  : m_tile_size(tile_size)
  , m_side_length(side_length)
  , m_axis(axis)
  , m_index(index)
  {
  }

  HexagonalCells::HexagonalCells(float radius, CellAxis axis, CellIndex index)
  : m_tile_size(compute_regular_size(axis, radius))
  , m_side_length(radius)
  , m_axis(axis)
  , m_index(index)
  {
  }

  RectF HexagonalCells::compute_bounds(Vec2I layer_size) const
  {
    Vec2F size = gf::vec(0.0f, 0.0f);
    const float offset = compute_offset(m_tile_size, m_side_length, m_axis);

    switch (m_axis) {
      case CellAxis::X:
        size.h = static_cast<float>(layer_size.h) * m_tile_size.h + m_tile_size.h / 2;
        size.w = static_cast<float>(layer_size.w) * (m_tile_size.w - offset) + offset;
        break;

      case CellAxis::Y:
        size.w = static_cast<float>(layer_size.w) * m_tile_size.w + m_tile_size.w / 2;
        size.h = static_cast<float>(layer_size.h) * (m_tile_size.h - offset) + offset;
        break;
    }

    return RectF::from_size(size);
  }

  RectI HexagonalCells::compute_visible_area(RectF local) const
  {
    return RectI::from_min_max(compute_coordinates(local.min()), compute_coordinates(local.max()));
  }

  RectF HexagonalCells::compute_cell_bounds(Vec2I coordinates) const
  {
    Vec2F base = gf::vec(0.0f, 0.0f);
    const float offset = compute_offset(m_tile_size, m_side_length, m_axis);

    switch (m_axis) {
      case CellAxis::X:
        base.x = static_cast<float>(coordinates.x) * (m_tile_size.w - offset);
        base.y = static_cast<float>(coordinates.y) * m_tile_size.h;

        switch (m_index) {
          case CellIndex::Odd:
            if (coordinates.x % 2 != 0) {
              base.y += m_tile_size.h / 2;
            }
            break;
          case CellIndex::Even:
            if (coordinates.x % 2 == 0) {
              base.y += m_tile_size.h / 2;
            }
            break;
        }
        break;

      case CellAxis::Y:
        base.y = static_cast<float>(coordinates.y) * (m_tile_size.h - offset);
        base.x = static_cast<float>(coordinates.x) * m_tile_size.w;

        switch (m_index) {
          case CellIndex::Odd:
            if (coordinates.y % 2 != 0) {
              base.x += m_tile_size.w / 2;
            }
            break;
          case CellIndex::Even:
            if (coordinates.y % 2 == 0) {
              base.x += m_tile_size.w / 2;
            }
            break;
        }
        break;
    }

    return RectF::from_position_size(base, m_tile_size);
  }

  Vec2I HexagonalCells::compute_coordinates(Vec2F position) const
  {
    // TODO: good approximation but would need some tweaking
    Vec2I coordinates = gf::vec(0, 0);
    const float offset = compute_offset(m_tile_size, m_side_length, m_axis);

    switch (m_axis) {
      case CellAxis::X:
        coordinates.x = static_cast<int>(position.x / (m_tile_size.w - offset));
        switch (m_index) {
          case CellIndex::Odd:
            if (coordinates.x % 2 == 0) {
              coordinates.y = static_cast<int>(position.y / m_tile_size.h);
            } else {
              coordinates.y = static_cast<int>((position.y - m_tile_size.h / 2) / m_tile_size.h);
            }
            break;
          case CellIndex::Even:
            if (coordinates.x % 2 != 0) {
              coordinates.y = static_cast<int>(position.y / m_tile_size.h);
            } else {
              coordinates.y = static_cast<int>((position.y - m_tile_size.h / 2) / m_tile_size.h);
            }
            break;
        }
        break;
      case CellAxis::Y:
        coordinates.y = static_cast<int>(position.y / (m_tile_size.h - offset));
        switch (m_index) {
          case CellIndex::Odd:
            if (coordinates.y % 2 == 0) {
              coordinates.x = static_cast<int>(position.x / m_tile_size.w);
            } else {
              coordinates.x = static_cast<int>((position.x - m_tile_size.w / 2) / m_tile_size.w);
            }
            break;
          case CellIndex::Even:
            if (coordinates.y % 2 != 0) {
              coordinates.x = static_cast<int>(position.x / m_tile_size.w);
            } else {
              coordinates.x = static_cast<int>((position.x - m_tile_size.w / 2) / m_tile_size.w);
            }
            break;
        }
        break;
    }

    return coordinates;
  }

  std::vector<Vec2F> HexagonalCells::compute_contour(Vec2I coordinates) const
  {
    const float offset = compute_offset(m_tile_size, m_side_length, m_axis);
    const RectF bounds = compute_cell_bounds(coordinates);
    const Vec2F min = bounds.min();
    const Vec2F max = bounds.max();

    std::vector<Vec2F> contour;

    switch (m_axis) {
      case CellAxis::X:
        // clang-format off
        contour.emplace_back(min.x,           (min.y + max.y) / 2);
        contour.emplace_back(min.x + offset,  min.y);
        contour.emplace_back(max.x - offset,  min.y);
        contour.emplace_back(max.x,           (min.y + max.y) / 2);
        contour.emplace_back(max.x - offset,  max.y);
        contour.emplace_back(min.x + offset,  max.y);
        // clang-format on
        break;

      case CellAxis::Y:
        // clang-format off
        contour.emplace_back((min.x + max.x) / 2, min.y);
        contour.emplace_back(min.x,               min.y + offset);
        contour.emplace_back(min.x,               max.y - offset);
        contour.emplace_back((min.x + max.x) / 2, max.y);
        contour.emplace_back(max.x,               max.y - offset);
        contour.emplace_back(max.x,               min.y + offset);
        // clang-format on
        break;
    }

    return contour;
  }

  std::vector<Vec2I> HexagonalCells::compute_neighbors(Vec2I coordinates, Vec2I layer_size, Flags<CellNeighborQuery> flags) const
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
        if ((m_index == CellIndex::Odd && coordinates.x % 2 == 0) || (m_index == CellIndex::Even && coordinates.x % 2 != 0)) {
          relative = XOffsets[0];
        } else {
          relative = XOffsets[1];
        }
        break;
      case CellAxis::Y:
        if ((m_index == CellIndex::Odd && coordinates.y % 2 == 0) || (m_index == CellIndex::Even && coordinates.y % 2 != 0)) {
          relative = YOffsets[0];
        } else {
          relative = YOffsets[1];
        }
        break;
    }

    std::vector<Vec2I> neighbors;

    for (auto offset : relative) {
      neighbors.push_back(coordinates + offset);
    }

    assert(neighbors.size() == 6);

    if (flags.test(CellNeighborQuery::Valid)) {
      const RectI bounds = RectI::from_size(layer_size);
      // clang-format off
      neighbors.erase(std::remove_if(neighbors.begin(), neighbors.end(), [bounds](Vec2I neighbor) {
        return !bounds.contains(neighbor);
      }), neighbors.end());
      // clang-format on
    }

    return neighbors;
  }

  Vec2F HexagonalCells::compute_regular_size(CellAxis axis, float radius)
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

  /*
   * Cells
   */

  Cells::Cells(OrthogonalCells cells)
  : m_variant(cells)
  {
  }

  Cells::Cells(StaggeredCells cells)
  : m_variant(cells)
  {
  }

  Cells::Cells(HexagonalCells cells)
  : m_variant(cells)
  {
  }

  Cells Cells::make_orthogonal(Vec2F tile_size)
  {
    return { OrthogonalCells(tile_size) };
  }

  Cells Cells::make_staggered(Vec2F tile_size, CellAxis axis, CellIndex index)
  {
    return { StaggeredCells(tile_size, axis, index) };
  }

  Cells Cells::make_hexagonal(Vec2F tile_size, float side_length, CellAxis axis, CellIndex index)
  {
    return { HexagonalCells(tile_size, side_length, axis, index) };
  }

  Cells Cells::make_hexagonal(float radius, CellAxis axis, CellIndex index)
  {
    return { HexagonalCells(radius, axis, index) };
  }

  RectF Cells::compute_bounds(Vec2I layer_size) const
  {
    return std::visit([=](auto&& cells) {
      using T = std::decay_t<decltype(cells)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        return RectF::from_size({ 0.0f, 0.0f });
      } else {
        return cells.compute_bounds(layer_size);
      }
    },
        m_variant);
  }

  RectI Cells::compute_visible_area(RectF local) const
  {
    return std::visit([=](auto&& cells) {
      using T = std::decay_t<decltype(cells)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        return RectI::from_size({ 0, 0 });
      } else {
        return cells.compute_visible_area(local);
      }
    },
        m_variant);
  }

  RectF Cells::compute_cell_bounds(Vec2I coordinates) const
  {
    return std::visit([=](auto&& cells) {
      using T = std::decay_t<decltype(cells)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        return RectF::from_size({ 0.0f, 0.0f });
      } else {
        return cells.compute_cell_bounds(coordinates);
      }
    },
        m_variant);
  }

  Vec2I Cells::compute_coordinates(Vec2F position) const
  {
    return std::visit([=](auto&& cells) {
      using T = std::decay_t<decltype(cells)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        return Vec2I(0, 0);
      } else {
        return cells.compute_coordinates(position);
      }
    },
        m_variant);
  }

  std::vector<Vec2F> Cells::compute_contour(Vec2I coordinates) const
  {
    return std::visit([=](auto&& cells) {
      using T = std::decay_t<decltype(cells)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        std::vector<Vec2F> empty;
        return empty;
      } else {
        return cells.compute_contour(coordinates);
      }
    },
        m_variant);
  }

  std::vector<Vec2I> Cells::compute_neighbors(Vec2I coordinates, Vec2I layer_size, Flags<CellNeighborQuery> flags) const
  {
    return std::visit([=](auto&& cells) {
      using T = std::decay_t<decltype(cells)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        std::vector<Vec2I> empty;
        return empty;
      } else {
        return cells.compute_neighbors(coordinates, layer_size, flags);
      }
    },
        m_variant);
  }

} // namespace gf
