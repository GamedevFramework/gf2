// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_MATH_H
#define GF_MATH_H

#include <cmath>
#include <limits>
#include <type_traits>

namespace gf {

  namespace constants {

    template<typename T>
    constexpr
    T Pi = T(3.141592653589793238462643383279502884197169399L);

    template<typename T>
    constexpr
    T Sqrt2 = T(1.4142135623730950488016887242096980785696718753769480731L);

    template<typename T>
    constexpr
    T Sqrt3 = T(1.7320508075688772935274463415058723669428052538103806280L);

  }

  constexpr float Pi = constants::Pi<float>;
  constexpr float Pi2 = Pi / 2;
  constexpr float Pi3 = Pi / 3;
  constexpr float Pi4 = Pi / 4;
  constexpr float Pi6 = Pi / 6;
  constexpr float Sqrt2 = constants::Sqrt2<float>;
  constexpr float InvSqrt2 = 1 / Sqrt2;
  constexpr float Sqrt3 = constants::Sqrt3<float>;
  constexpr float InvSqrt3 = 1 / Sqrt3;
  constexpr float Epsilon = std::numeric_limits<float>::epsilon();

  template<typename T>
  constexpr
  bool almost_equals(T a, T b, T abs_error = std::numeric_limits<T>::epsilon(), T rel_error = std::numeric_limits<T>::epsilon()) {
    static_assert(std::is_floating_point_v<T>, "Comparison should be between two floats.");

    if (a == b) {
      return true;
    }

    const T diff = std::abs(a - b);

    if (diff <= abs_error) {
      return true;
    }

    const double max = std::min(std::max(std::abs(a), std::abs(b)), std::numeric_limits<T>::max());
    return diff <= max * rel_error;
  }

  template<typename T>
  constexpr
  std::enable_if_t<std::is_floating_point_v<T>, T> degrees_to_radians(T degrees) {
    return degrees * constants::Pi<T> / T(180);
  }

  template<typename T>
  constexpr
  std::enable_if_t<std::is_floating_point_v<T>, T> radians_to_degrees(T radians) {
    return radians * T(180) / constants::Pi<T>;
  }

  template<typename T>
  using Step = T (*)(T);

  template<typename T>
  constexpr
  T linear_step(T t) {
    return t;
  }

  template<typename T>
  constexpr
  T cubic_step(T t) {
    return (-T(2) * t + T(3)) * t * t;
  }

  template<typename T>
  constexpr
  T quintic_step(T t) {
    return ((T(6) * t - T(15)) * t + T(10)) * t * t * t;
  }

  template<typename T>
  inline
  T cosine_step(T t) {
    return (T(1) - std::cos(constants::Pi<T> * t)) * T(0.5);
  }

  template<typename T, typename U>
  constexpr
  T lerp(T lhs, T rhs, U t) {
    return T((U(1) - t) * lhs + t * rhs);
  }

  template<typename T>
  constexpr
  T clamp(T val, T lo, T hi) {
    return val < lo ? lo : (hi < val ? hi : val);
  }

  template<typename T>
  constexpr
  T square(T val) {
    return val * val;
  }

  template<typename T>
  constexpr
  T cube(T val) {
    return val * val * val;
  }

  template<typename T>
  constexpr
  int sign(T val) {
    return (val > T(0)) - (val < T(0));
  }

  template<typename T>
  constexpr
  T absdiff(T lhs, T rhs) {
    return lhs > rhs ? lhs - rhs : rhs - lhs;
  }

}

#endif // GF_MATH_H
