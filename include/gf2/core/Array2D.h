// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_ARRAY_2D_H
#define GF_ARRAY_2D_H

#include <cassert>

#include <vector>

#include "Range.h"
#include "Vec2.h"

namespace gf {

  namespace details {

    template<typename Archive, typename Self>
    Archive& handle_array2d_serialization(Archive& ar, Self& self)
    {
      return ar | self.m_size | self.m_data;
    }

  }

  template<typename T>
  class Array2D {
  public:
    Array2D()
    : m_size(0, 0)
    {
    }

    Array2D(Vec2I size)
    : m_size(size)
    , m_data(static_cast<std::size_t>(size.x) * static_cast<std::size_t>(size.y))
    {
    }

    Array2D(Vec2I size, const T& value)
    : m_size(size)
    , m_data(static_cast<std::size_t>(size.x) * static_cast<std::size_t>(size.y), value)
    {
    }

    Vec2I size() const noexcept
    {
      return m_size;
    }

    bool empty() const noexcept
    {
      return m_data.empty();
    }

    bool valid(Vec2I index) const noexcept
    {
      return 0 <= index.x && index.x < m_size.x && 0 <= index.y && index.y < m_size.y;
    }

    IndexRange index_range() const noexcept
    {
      return gf::index_range(m_data.size());
    }

    PositionRange position_range() const noexcept
    {
      return gf::position_range(m_size);
    }

    const T& operator()(Vec2I position) const noexcept
    {
      assert(valid(position));
      return m_data[linearize(position)];
    }

    T& operator()(Vec2I position) noexcept
    {
      assert(valid(position));
      return m_data[linearize(position)];
    }

    const T& operator[](std::size_t index) const noexcept
    {
      assert(index < m_data.size());
      return m_data[index];
    }

    T& operator[](std::size_t index) noexcept
    {
      assert(index < m_data.size());
      return m_data[index];
    }

    const T* raw_data() const noexcept
    {
      return m_data.data();
    }

    std::size_t raw_size() const noexcept
    {
      return m_data.size();
    }

    const T* begin() const noexcept { return m_data.data(); }
    const T* end() const noexcept { return m_data.data() + m_data.size(); }

    T* begin() noexcept { return m_data.data(); }
    T* end() noexcept { return m_data.data() + m_data.size(); }

    auto compute_4_neighbors_range(Vec2I position) const noexcept
    {
      return diamond_range(position, 1);
    }

    auto compute_8_neighbors_range(Vec2I position) const noexcept
    {
      return square_range(position, 1);
    }

    auto compute_12_neighbors_range(Vec2I position) const noexcept
    {
      return diamond_range(position, 2);
    }

    auto compute_24_neighbors_range(Vec2I position) const noexcept
    {
      return square_range(position, 2);
    }

    NeighborSquareRange<int> square_range(Vec2I position, int radius) const noexcept
    {
      assert(valid(position));

      auto dx = range(position.x - std::min(position.x, radius), position.x + std::min(m_size.w - position.x - 1, radius) + 1);
      auto dy = range(position.y - std::min(position.y, radius), position.y + std::min(m_size.h - position.y - 1, radius) + 1);

      return { dx, dy, position };
    }

    NeighborDiamondRange<int> diamond_range(Vec2I position, int radius) const noexcept
    {
      assert(valid(position));

      auto dx = range(position.x - std::min(position.x, radius), position.x + std::min(m_size.w - position.x - 1, radius) + 1);
      auto dy = range(position.y - std::min(position.y, radius), position.y + std::min(m_size.h - position.y - 1, radius) + 1);

      return { dx, dy, position, radius };
    }

  private:
    std::size_t linearize(Vec2I index) const
    {
      return static_cast<std::size_t>(index.x) + (static_cast<std::size_t>(index.y) * static_cast<std::size_t>(m_size.x));
    }

    template<typename Archive, typename Self>
    friend Archive& details::handle_array2d_serialization(Archive& ar, Self& self);

    Vec2I m_size;
    std::vector<T> m_data;
  };

  template<typename T>
  bool operator==(const Array2D<T>& lhs, const Array2D<T>& rhs)
  {
    if (lhs.size() != rhs.size()) {
      return false;
    }

    for (auto i : lhs.index_range()) {
      if (lhs[i] != rhs[i]) {
        return false;
      }
    }

    return true;
  }

  template<typename Archive, typename T>
  inline Archive& operator|(Archive& ar, Array2D<T>& vec)
  {
    return details::handle_array2d_serialization(ar, vec);
  }

  template<typename Archive, typename T>
  inline Archive& operator|(Archive& ar, const Array2D<T>& vec)
  {
    return details::handle_array2d_serialization(ar, vec);
  }

} // namespace gf

#endif // GF_ARRAY_2D_H
