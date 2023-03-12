// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_VEC3_H
#define GF_VEC3_H

#include <cassert>
#include <cstddef>
#include <type_traits>

#include "CoreApi.h"
#include "Math.h"

namespace gf {

  template <typename T>
  struct Vec3 {
    Vec3() = default;

    constexpr
    Vec3(T x, T y, T z) noexcept
    : x(x)
    , y(y)
    , z(z)
    {
    }

    Vec3(const Vec3& other) = default;

    template<typename U>
    constexpr
    Vec3(Vec3<U> other) noexcept
    : x(static_cast<T>(other.x))
    , y(static_cast<T>(other.y))
    , z(static_cast<T>(other.z))
    {
      static_assert(std::is_convertible_v<U, T>, "Non-convertible types");
    }

    Vec3& operator=(const Vec3& other) = default;

    template<typename U>
    Vec3& operator=(Vec3<U> other) noexcept {
      static_assert(std::is_convertible_v<U, T>, "Non-convertible types");
      x = static_cast<T>(other.x);
      y = static_cast<T>(other.y);
      z = static_cast<T>(other.z);
      return *this;
    }

    T x;
    T y;
    T z;
  };

  using Vec3F = Vec3<float>;
  using Vec3D = Vec3<double>;
  using Vec3I = Vec3<int>;
  using Vec3U = Vec3<unsigned>;

// MSVC does not like extern template
#ifndef _MSC_VER
  extern template struct GF_CORE_API Vec3<float>;
  extern template struct GF_CORE_API Vec3<double>;
  extern template struct GF_CORE_API Vec3<int>;
  extern template struct GF_CORE_API Vec3<unsigned>;
#endif

  template<typename T>
  using Distance3 = T (*)(Vec3<T>, Vec3<T>);

  template<typename Tx, typename Ty, typename Tz>
  constexpr
  auto vec(Tx x, Ty y, Tz z) -> Vec3<std::common_type_t<Tx, Ty, Tz>> {
    using T = std::common_type_t<Tx, Ty, Tz>;
    return Vec3<T>(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z));
  }

  template<typename T>
  constexpr
  bool operator==(Vec3<T> lhs, Vec3<T> rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
  }

  template<typename T>
  constexpr
  bool operator!=(Vec3<T> lhs, Vec3<T> rhs) {
    return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
  }

  template<typename T>
  constexpr
  Vec3<T> operator-(Vec3<T> vec) {
    return Vec3<T>(-vec.x, -vec.y, -vec.z);
  }

  /*
   * Addition
   */

  template<typename T, typename U>
  constexpr
  Vec3<std::common_type_t<T, U>> operator+(Vec3<T> lhs, Vec3<U> rhs) {
    return Vec3<std::common_type_t<T, U>>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
  }

  template<typename T, typename U>
  constexpr
  Vec3<T>& operator+=(Vec3<T>& lhs, Vec3<U> rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
  }

  template<typename T, typename U>
  constexpr
  Vec3<std::common_type_t<T, U>> operator+(Vec3<T> lhs, U rhs) {
    return Vec3<std::common_type_t<T, U>>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
  }

  template<typename T, typename U>
  constexpr
  Vec3<T>& operator+=(Vec3<T>& lhs, U rhs) {
    lhs.x += rhs;
    lhs.y += rhs;
    lhs.z += rhs;
    return lhs;
  }

  template<typename T, typename U>
  constexpr
  Vec3<std::common_type_t<T, U>> operator+(T lhs, Vec3<U> rhs) {
    return Vec3<std::common_type_t<T, U>>(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
  }

  /*
   * Substraction
   */

  template<typename T, typename U>
  constexpr
  Vec3<std::common_type_t<T, U>> operator-(Vec3<T> lhs, Vec3<U> rhs) {
    return Vec3<std::common_type_t<T, U>>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
  }

  template<typename T, typename U>
  constexpr
  Vec3<T>& operator-=(Vec3<T>& lhs, Vec3<U> rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
  }

  template<typename T, typename U>
  constexpr
  Vec3<std::common_type_t<T, U>> operator-(Vec3<T> lhs, U rhs) {
    return Vec3<std::common_type_t<T, U>>(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
  }

  template<typename T, typename U>
  constexpr
  Vec3<T>& operator-=(Vec3<T>& lhs, U rhs) {
    lhs.x -= rhs;
    lhs.y -= rhs;
    lhs.z -= rhs;
    return lhs;
  }

  template<typename T, typename U>
  constexpr
  Vec3<std::common_type_t<T, U>> operator-(T lhs, Vec3<U> rhs) {
    return Vec3<std::common_type_t<T, U>>(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
  }

  /*
   * Multiplication
   */

  template<typename T, typename U>
  constexpr
  Vec3<std::common_type_t<T, U>> operator*(Vec3<T> lhs, Vec3<U> rhs) {
    return Vec3<std::common_type_t<T, U>>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
  }

  template<typename T, typename U>
  constexpr
  Vec3<T>& operator*=(Vec3<T>& lhs, Vec3<U> rhs) {
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    return lhs;
  }

  template<typename T, typename U>
  constexpr
  Vec3<std::common_type_t<T, U>> operator*(Vec3<T> lhs, U rhs) {
    return Vec3<std::common_type_t<T, U>>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
  }

  template<typename T, typename U>
  constexpr
  Vec3<T>& operator*=(Vec3<T>& lhs, U rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    return lhs;
  }

  template<typename T, typename U>
  constexpr
  Vec3<std::common_type_t<T, U>> operator*(T lhs, Vec3<U> rhs) {
    return Vec3<std::common_type_t<T, U>>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
  }

  /*
   * Division
   */

  template<typename T, typename U>
  constexpr
  Vec3<std::common_type_t<T, U>> operator/(Vec3<T> lhs, Vec3<U> rhs) {
    return Vec3<std::common_type_t<T, U>>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
  }

  template<typename T, typename U>
  constexpr
  Vec3<T>& operator/=(Vec3<T>& lhs, Vec3<U> rhs) {
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    lhs.z /= rhs.z;
    return lhs;
  }

  template<typename T, typename U>
  constexpr
  Vec3<std::common_type_t<T, U>> operator/(Vec3<T> lhs, U rhs) {
    return Vec3<std::common_type_t<T, U>>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
  }

  template<typename T, typename U>
  constexpr
  Vec3<T>& operator/=(Vec3<T>& lhs, U rhs) {
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;
    return lhs;
  }

  template<typename T, typename U>
  constexpr
  Vec3<std::common_type_t<T, U>> operator/(T lhs, Vec3<U> rhs) {
    return Vec3<std::common_type_t<T, U>>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
  }

  /*
   * Functions
   */

  template<typename T>
  constexpr
  T dot(Vec3<T> lhs, Vec3<T> rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
  }

  template<typename T>
  constexpr
  T manhattan_length(Vec3<T> vec) {
    return details::abs(vec.x) + details::abs(vec.y) + details::abs(vec.z);
  }

  template<typename T>
  constexpr
  T manhattan_distance(Vec3<T> lhs, Vec3<T> rhs) {
    return manhattan_length(lhs - rhs);
  }

  template<typename T>
  constexpr
  T square_length(Vec3<T> vec) {
    return square(vec.x) + square(vec.y) + square(vec.z);
  }

  template<typename T>
  constexpr
  T square_distance(Vec3<T> lhs, Vec3<T> rhs) {
    return square_length(lhs - rhs);
  }

  template<typename T>
  inline
  auto euclidean_length(Vec3<T> vec) {
    return std::hypot(vec.x, vec.y, vec.z);
  }

  template<typename T>
  inline
  auto euclidean_distance(Vec3<T> lhs, Vec3<T> rhs) {
    return euclidean_length(lhs - rhs);
  }

  template<typename T>
  constexpr
  T chebyshev_length(Vec3<T> vec) {
    return std::max(details::abs(vec.x), std::max(details::abs(vec.y), details::abs(vec.z)));
  }

  template<typename T>
  constexpr
  T chebyshev_distance(Vec3<T> lhs, Vec3<T> rhs) {
    return chebyshev_length(lhs - rhs);
  }

  template<typename T>
  constexpr
  T natural_length(Vec3<T> vec) {
    return manhattan_length(vec) + square_length(vec);
  }

  template<typename T>
  constexpr
  T natural_distance(Vec3<T> lhs, Vec3<T> rhs) {
    return natural_length(lhs - rhs);
  }

  template<typename T>
  inline
  Vec3<T> normalize(Vec3<T> vec) {
    return vec / euclidean_length(vec);
  }

  template<typename T>
  constexpr
  Vec3<T> projx(Vec3<T> vec) {
    return { vec.x, T(0), T(0) };
  }

  template<typename T>
  constexpr
  Vec3<T> projy(Vec3<T> vec) {
    return { T(0), vec.y, T(0) };
  }

  template<typename T>
  constexpr
  Vec3<T> projz(Vec3<T> vec) {
    return { T(0), T(0), vec.z };
  }

  template<typename T>
  constexpr
  Vec3<T> cross(Vec3<T> lhs, Vec3<T> rhs) {
    return {
      lhs.y * rhs.z - lhs.z * rhs.y,
      lhs.z * rhs.x - lhs.x * rhs.z,
      lhs.x * rhs.y - lhs.y * rhs.x
    };
  }

  template<typename Archive, typename T>
  inline
  Archive& operator|(Archive& ar, Vec3<T>& vec) {
    return ar | vec.x | vec.y | vec.z;
  }

}

#endif // GF_VEC3_H
