// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_VEC2_H
#define GF_VEC2_H

#include <cassert>
#include <cstdint>

#include <type_traits>

#include "CoreApi.h"
#include "Math.h"

namespace gf {

  template<typename T>
  struct Vec2 {
    Vec2() = default;
    ~Vec2() = default;

    constexpr Vec2(T x, T y) noexcept
    : x(x)
    , y(y)
    {
    }

    Vec2(const Vec2& other) = default;

    template<typename U>
    constexpr Vec2(Vec2<U> other) noexcept
    : x(static_cast<T>(other.x))
    , y(static_cast<T>(other.y))
    {
      static_assert(std::is_convertible_v<U, T>, "Non-convertible types");
    }

    Vec2& operator=(const Vec2& other) = default;

    template<typename U>
    Vec2& operator=(Vec2<U> other) noexcept
    {
      static_assert(std::is_convertible_v<U, T>, "Non-convertible types");
      x = static_cast<T>(other.x);
      y = static_cast<T>(other.y);
      return *this;
    }

    Vec2(Vec2&& other) noexcept = default;
    Vec2& operator=(Vec2&& other) noexcept = default;

    union {
      T x;
      T u;
      T w;
    };

    union {
      T y;
      T v;
      T h;
    };
  };

  using Vec2F = Vec2<float>;
  using Vec2D = Vec2<double>;
  using Vec2I = Vec2<int32_t>;
  using Vec2U = Vec2<uint32_t>;

// MSVC does not like extern template
#ifndef _MSC_VER
  extern template struct GF_CORE_API Vec2<float>;
  extern template struct GF_CORE_API Vec2<double>;
  extern template struct GF_CORE_API Vec2<int32_t>;
  extern template struct GF_CORE_API Vec2<uint32_t>;
#endif

  template<typename T>
  using Distance2 = T (*)(Vec2<T>, Vec2<T>);

  template<typename Tx, typename Ty>
  constexpr auto vec(Tx x, Ty y) -> Vec2<std::common_type_t<Tx, Ty>>
  {
    using T = std::common_type_t<Tx, Ty>;
    return Vec2<T>(static_cast<T>(x), static_cast<T>(y));
  }

  template<typename T>
  constexpr bool operator==(Vec2<T> lhs, Vec2<T> rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }

  template<typename T>
  constexpr bool operator!=(Vec2<T> lhs, Vec2<T> rhs)
  {
    return lhs.x != rhs.x || lhs.y != rhs.y;
  }

  template<typename T>
  constexpr Vec2<T> operator-(Vec2<T> vec)
  {
    return Vec2<T>(-vec.x, -vec.y);
  }

  /*
   * Addition
   */

  template<typename T, typename U>
  constexpr Vec2<std::common_type_t<T, U>> operator+(Vec2<T> lhs, Vec2<U> rhs)
  {
    return Vec2<std::common_type_t<T, U>>(lhs.x + rhs.x, lhs.y + rhs.y);
  }

  template<typename T, typename U>
  constexpr Vec2<T>& operator+=(Vec2<T>& lhs, Vec2<U> rhs)
  {
    lhs.x += static_cast<T>(rhs.x);
    lhs.y += static_cast<T>(rhs.y);
    return lhs;
  }

  template<typename T, typename U>
  constexpr Vec2<std::common_type_t<T, U>> operator+(Vec2<T> lhs, U rhs)
  {
    return Vec2<std::common_type_t<T, U>>(lhs.x + rhs, lhs.y + rhs);
  }

  template<typename T, typename U>
  constexpr Vec2<T>& operator+=(Vec2<T>& lhs, U rhs)
  {
    lhs.x += static_cast<T>(rhs);
    lhs.y += static_cast<T>(rhs);
    return lhs;
  }

  template<typename T, typename U>
  constexpr Vec2<std::common_type_t<T, U>> operator+(T lhs, Vec2<U> rhs)
  {
    return Vec2<std::common_type_t<T, U>>(lhs + rhs.x, lhs + rhs.y);
  }

  /*
   * Substraction
   */

  template<typename T, typename U>
  constexpr Vec2<std::common_type_t<T, U>> operator-(Vec2<T> lhs, Vec2<U> rhs)
  {
    return Vec2<std::common_type_t<T, U>>(lhs.x - rhs.x, lhs.y - rhs.y);
  }

  template<typename T, typename U>
  constexpr Vec2<T>& operator-=(Vec2<T>& lhs, Vec2<U> rhs)
  {
    lhs.x -= static_cast<T>(rhs.x);
    lhs.y -= static_cast<T>(rhs.y);
    return lhs;
  }

  template<typename T, typename U>
  constexpr Vec2<std::common_type_t<T, U>> operator-(Vec2<T> lhs, U rhs)
  {
    return Vec2<std::common_type_t<T, U>>(lhs.x - rhs, lhs.y - rhs);
  }

  template<typename T, typename U>
  constexpr Vec2<T>& operator-=(Vec2<T>& lhs, U rhs)
  {
    lhs.x -= static_cast<T>(rhs);
    lhs.y -= static_cast<T>(rhs);
    return lhs;
  }

  template<typename T, typename U>
  constexpr Vec2<std::common_type_t<T, U>> operator-(T lhs, Vec2<U> rhs)
  {
    return Vec2<std::common_type_t<T, U>>(lhs - rhs.x, lhs - rhs.y);
  }

  /*
   * Multiplication
   */

  template<typename T, typename U>
  constexpr Vec2<std::common_type_t<T, U>> operator*(Vec2<T> lhs, Vec2<U> rhs)
  {
    return Vec2<std::common_type_t<T, U>>(lhs.x * rhs.x, lhs.y * rhs.y);
  }

  template<typename T, typename U>
  constexpr Vec2<T>& operator*=(Vec2<T>& lhs, Vec2<U> rhs)
  {
    lhs.x *= static_cast<T>(rhs.x);
    lhs.y *= static_cast<T>(rhs.y);
    return lhs;
  }

  template<typename T, typename U>
  constexpr Vec2<std::common_type_t<T, U>> operator*(Vec2<T> lhs, U rhs)
  {
    return Vec2<std::common_type_t<T, U>>(lhs.x * rhs, lhs.y * rhs);
  }

  template<typename T, typename U>
  constexpr Vec2<T>& operator*=(Vec2<T>& lhs, U rhs)
  {
    lhs.x *= static_cast<T>(rhs);
    lhs.y *= static_cast<T>(rhs);
    return lhs;
  }

  template<typename T, typename U>
  constexpr Vec2<std::common_type_t<T, U>> operator*(T lhs, Vec2<U> rhs)
  {
    return Vec2<std::common_type_t<T, U>>(lhs * rhs.x, lhs * rhs.y);
  }

  /*
   * Division
   */

  template<typename T, typename U>
  constexpr Vec2<std::common_type_t<T, U>> operator/(Vec2<T> lhs, Vec2<U> rhs)
  {
    return Vec2<std::common_type_t<T, U>>(lhs.x / rhs.x, lhs.y / rhs.y);
  }

  template<typename T, typename U>
  constexpr Vec2<T>& operator/=(Vec2<T>& lhs, Vec2<U> rhs)
  {
    lhs.x /= static_cast<T>(rhs.x);
    lhs.y /= static_cast<T>(rhs.y);
    return lhs;
  }

  template<typename T, typename U>
  constexpr Vec2<std::common_type_t<T, U>> operator/(Vec2<T> lhs, U rhs)
  {
    return Vec2<std::common_type_t<T, U>>(lhs.x / rhs, lhs.y / rhs);
  }

  template<typename T, typename U>
  constexpr Vec2<T>& operator/=(Vec2<T>& lhs, U rhs)
  {
    lhs.x /= static_cast<T>(rhs);
    lhs.y /= static_cast<T>(rhs);
    return lhs;
  }

  template<typename T, typename U>
  constexpr Vec2<std::common_type_t<T, U>> operator/(T lhs, Vec2<U> rhs)
  {
    return Vec2<std::common_type_t<T, U>>(lhs / rhs.x, lhs / rhs.y);
  }

  /*
   * Functions
   */

  template<typename T>
  constexpr Vec2<T> min(Vec2<T> lhs, Vec2<T> rhs)
  {
    return { details::min(lhs.x, rhs.x), details::min(lhs.y, rhs.y) };
  }

  template<typename T>
  constexpr Vec2<T> max(Vec2<T> lhs, Vec2<T> rhs)
  {
    return { details::max(lhs.x, rhs.x), details::max(lhs.y, rhs.y) };
  }

  template<typename T>
  constexpr T dot(Vec2<T> lhs, Vec2<T> rhs)
  {
    return lhs.x * rhs.x + lhs.y * rhs.y;
  }

  template<typename T>
  constexpr T manhattan_length(Vec2<T> vec)
  {
    return details::abs(vec.x) + details::abs(vec.y);
  }

  template<typename T>
  constexpr T manhattan_distance(Vec2<T> lhs, Vec2<T> rhs)
  {
    return details::abs(lhs.x - rhs.x) + details::abs(lhs.y - rhs.y);
  }

  template<typename T>
  constexpr T square_length(Vec2<T> vec)
  {
    return square(vec.x) + square(vec.y);
  }

  template<typename T>
  constexpr T square_distance(Vec2<T> lhs, Vec2<T> rhs)
  {
    return square(lhs.x - rhs.x) + square(lhs.y - rhs.y);
  }

  template<typename T>
  inline auto euclidean_length(Vec2<T> vec)
  {
    return std::sqrt(square_length(vec));
  }

  template<typename T>
  inline auto euclidean_distance(Vec2<T> lhs, Vec2<T> rhs)
  {
    return std::sqrt(square_distance(lhs, rhs));
  }

  template<typename T>
  constexpr T chebyshev_length(Vec2<T> vec)
  {
    return details::max(details::abs(vec.x), details::abs(vec.y));
  }

  template<typename T>
  constexpr T chebyshev_distance(Vec2<T> lhs, Vec2<T> rhs)
  {
    return details::max(details::abs(lhs.x - rhs.x), details::abs(lhs.y - rhs.y));
  }

  template<typename T>
  constexpr T natural_length(Vec2<T> vec)
  {
    return manhattan_length(vec) + square_length(vec);
  }

  template<typename T>
  constexpr T natural_distance(Vec2<T> lhs, Vec2<T> rhs)
  {
    return natural_length(lhs - rhs);
  }

  template<typename T>
  inline Vec2<T> normalize(Vec2<T> vec)
  {
    return vec / euclidean_length(vec);
  }

  template<typename T>
  constexpr Vec2<T> projx(Vec2<T> vec)
  {
    return { vec.x, T(0) };
  }

  template<typename T>
  constexpr Vec2<T> projy(Vec2<T> vec)
  {
    return { T(0), vec.y };
  }

  template<typename T>
  constexpr Vec2<T> dirx(T length)
  {
    return { length, T(0) };
  }

  template<typename T>
  constexpr Vec2<T> diry(T length)
  {
    return { T(0), length };
  }

  template<typename T>
  inline Vec2<T> unit(T angle)
  {
    return { std::cos(angle), std::sin(angle) };
  }

  template<typename T>
  inline T angle(Vec2<T> vec)
  {
    return std::atan2(vec.y, vec.x);
  }

  template<typename T>
  constexpr Vec2<T> perp(Vec2<T> vec)
  {
    return { -vec.y, vec.x };
  }

  template<typename T>
  constexpr Vec2<T> triple_product(Vec2<T> a, Vec2<T> b, Vec2<T> c)
  {
    return dot(a, c) * b - dot(a, b) * c;
  }

  template<typename T>
  constexpr Vec2<T> inverse_triple_product(Vec2<T> a, Vec2<T> b, Vec2<T> c)
  {
    return -dot(c, b) * a + dot(c, a) * b;
  }

  template<typename T>
  constexpr T cross(Vec2<T> lhs, Vec2<T> rhs)
  {
    return lhs.x * rhs.y - lhs.y * rhs.x;
  }

  template<typename T>
  inline T angle_to(Vec2<T> lhs, Vec2<T> rhs)
  {
    return std::atan2(cross(lhs, rhs), dot(lhs, rhs));
  }

  template<typename T>
  constexpr Vec2<T> project_onto(Vec2<T> vec, Vec2<T> axis)
  {
    return dot(vec, axis) / square_length(axis) * axis;
  }

  template<typename T>
  constexpr bool almost_equals(Vec2<T> a, Vec2<T> b, T abs_error = std::numeric_limits<T>::epsilon(), T rel_error = std::numeric_limits<T>::epsilon())
  {
    return almost_equals(a.x, b.x, abs_error, rel_error) && almost_equals(a.y, b.y, abs_error, rel_error);
  }

  template<typename T>
  constexpr Vec2<T> abs(Vec2<T> vec)
  {
    return { details::abs(vec.x), details::abs(vec.y) };
  }

  template<typename T>
  constexpr Vec2<T> clamp(Vec2<T> vec, Vec2<T> lo, Vec2<T> hi)
  {
    return { clamp(vec.x, lo.x, hi.x), clamp(vec.y, lo.y, hi.y) };
  }

  template<typename T>
  constexpr Vec2<T> clamp(Vec2<T> vec, T lo, T hi)
  {
    return { clamp(vec.x, lo, hi), clamp(vec.y, lo, hi) };
  }

  namespace details {

    template<typename Archive, typename Self>
    Archive& handle_vec2_serialization(Archive& ar, Self& self)
    {
      return ar | self.x | self.y;
    }

  }

  template<typename Archive, typename T>
  inline Archive& operator|(Archive& ar, Vec2<T>& vec)
  {
    return details::handle_vec2_serialization(ar, vec);
  }

  template<typename Archive, typename T>
  inline Archive& operator|(Archive& ar, const Vec2<T>& vec)
  {
    return details::handle_vec2_serialization(ar, vec);
  }

} // namespace gf

#endif // GF_VEC2_H
