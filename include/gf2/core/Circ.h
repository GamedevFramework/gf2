// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CIRC_H
#define GF_CIRC_H

#include <cstdint>

#include "CoreApi.h"
#include "Math.h"
#include "Vec2.h"

namespace gf {

  template<typename T>
  struct Circ {
    Vec2<T> center;
    T radius;

    Circ() = default;

    static constexpr Circ from_center_radius(Vec2<T> center, T radius) noexcept
    {
      return { center, radius };
    }

    static constexpr Circ from_radius(T radius) noexcept
    {
      return { { T(0), T(0) }, radius };
    }

    constexpr bool empty() const noexcept
    {
      return radius == T(0);
    }

    constexpr bool contains(Vec2<T> point) const noexcept
    {
      return square_distance(center, point) <= square(radius);
    }

    constexpr bool intersects(Circ other) const noexcept
    {
      return square_distance(center, other.center) <= square(radius + other.radius);
    }
  };

  using CircF = Circ<float>;
  using CircD = Circ<double>;
  using CircI = Circ<int32_t>;
  using CircU = Circ<uint32_t>;

  template<typename T>
  constexpr bool operator==(Circ<T> lhs, Circ<T> rhs)
  {
    return lhs.center == rhs.center && lhs.radius == rhs.radius;
  }

  template<typename T>
  constexpr bool operator!=(Circ<T> lhs, Circ<T> rhs)
  {
    return lhs.center != rhs.center || lhs.radius != rhs.radius;
  }

// MSVC does not like extern template
#ifndef _MSC_VER
  extern template struct GF_CORE_API Circ<float>;
  extern template struct GF_CORE_API Circ<double>;
  extern template struct GF_CORE_API Circ<int32_t>;
  extern template struct GF_CORE_API Circ<uint32_t>;
#endif

  namespace details {

    template<typename Archive, typename Self>
    Archive& handle_circ_serialization(Archive& ar, Self& self)
    {
      return ar | self.center | self.radius;
    }

  }

  template<typename Archive, typename T>
  inline Archive& operator|(Archive& ar, Circ<T>& circle)
  {
    return details::handle_circ_serialization(ar, circle);
  }

  template<typename Archive, typename T>
  inline Archive& operator|(Archive& ar, const Circ<T>& circle)
  {
    return details::handle_circ_serialization(ar, circle);
  }

} // namespace gf

#endif // GF_CIRC_H
