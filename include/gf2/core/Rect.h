// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_RECT_H
#define GF_RECT_H

#include <cstdint>

#include <optional>

#include "Orientation.h"
#include "Vec2.h"

namespace gf {

  template<typename T>
  struct Rect {
    Vec2<T> offset;
    Vec2<T> extent;

    Rect() = default;

    static constexpr Rect from_position_size(Vec2<T> position, Vec2<T> size) noexcept
    {
      return { position, size };
    }

    static constexpr Rect from_size(Vec2<T> size) noexcept
    {
      return { Vec2<T>(T(0), T(0)), size };
    }

    static constexpr Rect from_min_max(Vec2<T> min, Vec2<T> max) noexcept
    {
      return { min, max - min };
    }

    static constexpr Rect from_center_size(Vec2<T> center, Vec2<T> size) noexcept
    {
      return { center - size / T(2), size };
    }

    constexpr bool empty() const noexcept
    {
      return extent.x <= T(0) || extent.y <= T(0);
    }

    constexpr Vec2<T> position() const noexcept
    {
      return offset;
    }

    constexpr Vec2<T> size() const noexcept
    {
      return extent;
    }

    constexpr Vec2<T> min() const noexcept
    {
      return offset;
    }

    constexpr Vec2<T> max() const noexcept
    {
      return offset + extent;
    }

    constexpr Vec2<T> center() const noexcept
    {
      return offset + extent / 2;
    }

    constexpr bool contains(Vec2<T> point) const noexcept
    {
      return offset.x <= point.x && point.x < offset.x + extent.x && offset.y <= point.y && point.y < offset.y + extent.y;
    }

    constexpr bool contains(Rect other) const noexcept
    {
      // clang-format off
      return offset.x <= other.offset.x && other.offset.x + other.extent.x <= offset.x + extent.x
          && offset.y <= other.offset.y && other.offset.y + other.extent.y <= offset.y + extent.y;
      // clang-format on
    }

    constexpr bool intersects(Rect other) const noexcept
    {
      // clang-format off
      return offset.x + extent.x > other.offset.x && offset.x < other.offset.x + other.extent.x
         &&  offset.y + extent.y > other.offset.y && offset.y < other.offset.y + other.extent.y;
      // clang-format on
    }

    constexpr std::optional<Rect> intersection(Rect other) const noexcept
    {
      if (!intersects(other)) {
        return std::nullopt;
      }

      Vec2<T> min(T(0), T(0));
      min.x = details::max(offset.x, other.offset.x);
      min.y = details::max(offset.y, other.offset.y);

      Vec2<T> max(T(0), T(0));
      max.x = details::min(offset.x + extent.x, other.offset.x + other.extent.x);
      max.y = details::min(offset.y + extent.y, other.offset.y + other.extent.y);

      return from_min_max(min, max);
    }

    constexpr Rect& extend_to(Vec2<T> point) noexcept
    {
      if (point.x < offset.x) {
        extent.x += (offset.x - point.x);
        offset.x = point.x;
      } else if (point.x > offset.x + extent.x) {
        extent.x = point.x - offset.x;
      }

      if (point.y < offset.y) {
        extent.y += (offset.y - point.y);
        offset.y = point.y;
      } else if (point.y > offset.y + extent.y) {
        extent.y = point.y - offset.y;
      }

      return *this;
    }

    constexpr Rect& extend_to(Rect other) noexcept
    {
      return extend_to(other.min()).extend_to(other.max());
    }

    constexpr Vec2<T> position_at(Orientation orientation) const noexcept
    {
      switch (orientation) {
        case Orientation::NorthWest:
          return offset;
        case Orientation::North:
          return offset + gf::vec(extent.x / T(2), T(0));
        case Orientation::NorthEast:
          return offset + gf::vec(extent.x, T(0));
        case Orientation::West:
          return offset + gf::vec(T(0), extent.y / T(2));
        case Orientation::Center:
          return offset + extent / T(2);
        case Orientation::East:
          return offset + gf::vec(extent.x / T(2), extent.y / T(2));
        case Orientation::SouthWest:
          return offset + gf::vec(T(0), extent.y);
        case Orientation::South:
          return offset + gf::vec(extent.x / T(2), extent.y);
        case Orientation::SouthEast:
          return offset + extent;
      }

      return offset;
    }

    constexpr Rect grow_by(T value) const noexcept
    {
      return from_position_size(offset - value, extent + T(2) * value);
    }

    constexpr Rect shrink_by(T value) const noexcept
    {
      return from_position_size(offset + value, extent - T(2) * value);
    }
  };

  using RectF = Rect<float>;
  using RectD = Rect<double>;
  using RectI = Rect<int32_t>;
  using RectU = Rect<uint32_t>;

  template<typename T>
  constexpr bool operator==(Rect<T> lhs, Rect<T> rhs)
  {
    return lhs.offset == rhs.offset && lhs.extent == rhs.extent;
  }

  template<typename T>
  constexpr bool operator!=(Rect<T> lhs, Rect<T> rhs)
  {
    return lhs.offset != rhs.offset || lhs.extent != rhs.extent;
  }

// MSVC does not like extern template
#ifndef _MSC_VER
  extern template struct GF_CORE_API Rect<float>;
  extern template struct GF_CORE_API Rect<double>;
  extern template struct GF_CORE_API Rect<int32_t>;
  extern template struct GF_CORE_API Rect<uint32_t>;
#endif

  namespace details {

    template<typename Archive, typename Self>
    Archive& handle_rect_serialization(Archive& ar, Self& self)
    {
      return ar | self.offset | self.extent;
    }

  }

  template<typename Archive, typename T>
  inline Archive& operator|(Archive& ar, Rect<T>& rectangle)
  {
    return details::handle_rect_serialization(ar, rectangle);
  }

  template<typename Archive, typename T>
  inline Archive& operator|(Archive& ar, const Rect<T>& rectangle)
  {
    return details::handle_rect_serialization(ar, rectangle);
  }

} // namespace gf

#endif // GF_RECT_H
