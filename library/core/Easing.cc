// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Easing.h>

#include <cmath>

#include <gf2/core/Math.h>

namespace gf {

  namespace {

    template<typename T, Easing<T> Func>
    class GenericEasing {
    public:
      static T ease_in(T t)
      {
        return Func(t);
      }

      static T ease_out(T t)
      {
        return T(1) - Func(T(1) - t);
      }

      static T ease_in_out(T t)
      {
        return t < T(0.5) ? ease_in(T(2) * t) / T(2) : T(0.5) + (ease_out((T(2) * t) - T(1)) / T(2));
      }

      static T ease_out_in(T t)
      {
        return t < T(0.5) ? ease_out(T(2) * t) / T(2) : T(0.5) + (ease_in((T(2) * t) - T(1)) / T(2));
      }
    };

    template<typename T>
    constexpr T quad(T t)
    {
      return t * t;
    }

    template<typename T>
    constexpr T cubic(T t)
    {
      return t * t * t;
    }

    template<typename T>
    constexpr T quart(T t)
    {
      return t * t * t * t;
    }

    template<typename T>
    constexpr T quint(T t)
    {
      return t * t * t * t * t;
    }

    template<typename T>
    inline T circ(T t)
    {
      return T(1) - std::sqrt(T(1) - (t * t));
    }

    template<typename T>
    inline T sine(T t)
    {
      return (T(1) - std::cos(constants::Pi<T> / T(2) * t));
    }

    template<typename T>
    inline T back(T t)
    {
      static constexpr T C1 = T(1.70158);
      static constexpr T C2 = T(1) + C1;
      return t * t * ((C2 * t) - C1);
    }

    template<typename T>
    inline T bounce(T t)
    {
      static constexpr T C1 = T(1.0) / T(2.75);
      static constexpr T C2 = T(7.5625); // = 2.75f * 2.75f

      const T u = T(1) - t;

      if (u < T(1) * C1) {
        return T(1) - (C2 * u * u);
      }

      if (u < T(2) * C1) {
        return T(1) - ((C2 * gf::square(u - (T(1.5) * C1))) + T(0.75));
      }

      if (u < T(2.5) * C1) {
        return T(1) - ((C2 * gf::square(u - (T(2.25) * C1))) + T(0.9375));
      }

      return T(1) - ((C2 * gf::square(u - (T(2.625) * C1))) + T(0.984375));
    }

    template<typename T>
    inline T elastic(T t)
    {
      static constexpr T C0 = T(0.3);

      const T u = t - T(1);

      return -std::pow(T(2.0), T(10) * u) * std::sin((u - (C0 / T(4))) * T(2) * constants::Pi<T> / C0);
    }

    template<typename T>
    inline T expo(T t)
    {
      return t == T(0.0) ? T(0.0) : std::pow(T(2.0), T(10) * (t - T(1)));
    }

  } // anonymous namespace


  // float

  float ease_linear(float t)
  {
    return t;
  }

  float ease_smooth(float t)
  {
    return cubic_step(t);
  }

  float ease_smoother(float t)
  {
    return quintic_step(t);
  }

  float ease_in_sine(float t)
  {
    return GenericEasing<float, sine>::ease_in(t);
  }

  float ease_out_sine(float t)
  {
    return GenericEasing<float, sine>::ease_out(t);
  }

  float ease_in_out_sine(float t)
  {
    return GenericEasing<float, sine>::ease_in_out(t);
  }

  float ease_out_in_sine(float t)
  {
    return GenericEasing<float, sine>::ease_out_in(t);
  }

  float ease_in_quad(float t)
  {
    return GenericEasing<float, quad>::ease_in(t);
  }

  float ease_out_quad(float t)
  {
    return GenericEasing<float, quad>::ease_out(t);
  }

  float ease_in_out_quad(float t)
  {
    return GenericEasing<float, quad>::ease_in_out(t);
  }

  float ease_out_in_quad(float t)
  {
    return GenericEasing<float, quad>::ease_out_in(t);
  }

  float ease_in_cubic(float t)
  {
    return GenericEasing<float, cubic>::ease_in(t);
  }

  float ease_out_cubic(float t)
  {
    return GenericEasing<float, cubic>::ease_out(t);
  }

  float ease_in_out_cubic(float t)
  {
    return GenericEasing<float, cubic>::ease_in_out(t);
  }

  float ease_out_in_cubic(float t)
  {
    return GenericEasing<float, cubic>::ease_out_in(t);
  }

  float ease_in_quart(float t)
  {
    return GenericEasing<float, quart>::ease_in(t);
  }

  float ease_out_quart(float t)
  {
    return GenericEasing<float, quart>::ease_out(t);
  }

  float ease_in_out_quart(float t)
  {
    return GenericEasing<float, quart>::ease_in_out(t);
  }

  float ease_out_in_quart(float t)
  {
    return GenericEasing<float, quart>::ease_out_in(t);
  }

  float ease_in_quint(float t)
  {
    return GenericEasing<float, quint>::ease_in(t);
  }

  float ease_out_quint(float t)
  {
    return GenericEasing<float, quint>::ease_out(t);
  }

  float ease_in_out_quint(float t)
  {
    return GenericEasing<float, quint>::ease_in_out(t);
  }

  float ease_out_in_quint(float t)
  {
    return GenericEasing<float, quint>::ease_out_in(t);
  }

  float ease_in_circ(float t)
  {
    return GenericEasing<float, circ>::ease_in(t);
  }

  float ease_out_circ(float t)
  {
    return GenericEasing<float, circ>::ease_out(t);
  }

  float ease_in_out_circ(float t)
  {
    return GenericEasing<float, circ>::ease_in_out(t);
  }

  float ease_out_in_circ(float t)
  {
    return GenericEasing<float, circ>::ease_out_in(t);
  }

  float ease_in_back(float t)
  {
    return GenericEasing<float, back>::ease_in(t);
  }

  float ease_out_back(float t)
  {
    return GenericEasing<float, back>::ease_out(t);
  }

  float ease_in_out_back(float t)
  {
    return GenericEasing<float, back>::ease_in_out(t);
  }

  float ease_out_in_back(float t)
  {
    return GenericEasing<float, back>::ease_out_in(t);
  }

  float ease_in_bounce(float t)
  {
    return GenericEasing<float, bounce>::ease_in(t);
  }

  float ease_out_bounce(float t)
  {
    return GenericEasing<float, bounce>::ease_out(t);
  }

  float ease_in_out_bounce(float t)
  {
    return GenericEasing<float, bounce>::ease_in_out(t);
  }

  float ease_out_in_bounce(float t)
  {
    return GenericEasing<float, bounce>::ease_out_in(t);
  }

  float ease_in_elastic(float t)
  {
    return GenericEasing<float, elastic>::ease_in(t);
  }

  float ease_out_elastic(float t)
  {
    return GenericEasing<float, elastic>::ease_out(t);
  }

  float ease_in_out_elastic(float t)
  {
    return GenericEasing<float, elastic>::ease_in_out(t);
  }

  float ease_out_in_elastic(float t)
  {
    return GenericEasing<float, elastic>::ease_out_in(t);
  }

  float ease_in_expo(float t)
  {
    return GenericEasing<float, expo>::ease_in(t);
  }

  float ease_out_expo(float t)
  {
    return GenericEasing<float, expo>::ease_out(t);
  }

  float ease_in_out_expo(float t)
  {
    return GenericEasing<float, expo>::ease_in_out(t);
  }

  float ease_out_in_expo(float t)
  {
    return GenericEasing<float, expo>::ease_out_in(t);
  }

  // double

  double ease_linear(double t)
  {
    return t;
  }

  double ease_smooth(double t)
  {
    return cubic_step(t);
  }

  double ease_smoother(double t)
  {
    return quintic_step(t);
  }

  double ease_in_sine(double t)
  {
    return GenericEasing<double, sine>::ease_in(t);
  }

  double ease_out_sine(double t)
  {
    return GenericEasing<double, sine>::ease_out(t);
  }

  double ease_in_out_sine(double t)
  {
    return GenericEasing<double, sine>::ease_in_out(t);
  }

  double ease_out_in_sine(double t)
  {
    return GenericEasing<double, sine>::ease_out_in(t);
  }

  double ease_in_quad(double t)
  {
    return GenericEasing<double, quad>::ease_in(t);
  }

  double ease_out_quad(double t)
  {
    return GenericEasing<double, quad>::ease_out(t);
  }

  double ease_in_out_quad(double t)
  {
    return GenericEasing<double, quad>::ease_in_out(t);
  }

  double ease_out_in_quad(double t)
  {
    return GenericEasing<double, quad>::ease_out_in(t);
  }

  double ease_in_cubic(double t)
  {
    return GenericEasing<double, cubic>::ease_in(t);
  }

  double ease_out_cubic(double t)
  {
    return GenericEasing<double, cubic>::ease_out(t);
  }

  double ease_in_out_cubic(double t)
  {
    return GenericEasing<double, cubic>::ease_in_out(t);
  }

  double ease_out_in_cubic(double t)
  {
    return GenericEasing<double, cubic>::ease_out_in(t);
  }

  double ease_in_quart(double t)
  {
    return GenericEasing<double, quart>::ease_in(t);
  }

  double ease_out_quart(double t)
  {
    return GenericEasing<double, quart>::ease_out(t);
  }

  double ease_in_out_quart(double t)
  {
    return GenericEasing<double, quart>::ease_in_out(t);
  }

  double ease_out_in_quart(double t)
  {
    return GenericEasing<double, quart>::ease_out_in(t);
  }

  double ease_in_quint(double t)
  {
    return GenericEasing<double, quint>::ease_in(t);
  }

  double ease_out_quint(double t)
  {
    return GenericEasing<double, quint>::ease_out(t);
  }

  double ease_in_out_quint(double t)
  {
    return GenericEasing<double, quint>::ease_in_out(t);
  }

  double ease_out_in_quint(double t)
  {
    return GenericEasing<double, quint>::ease_out_in(t);
  }

  double ease_in_circ(double t)
  {
    return GenericEasing<double, circ>::ease_in(t);
  }

  double ease_out_circ(double t)
  {
    return GenericEasing<double, circ>::ease_out(t);
  }

  double ease_in_out_circ(double t)
  {
    return GenericEasing<double, circ>::ease_in_out(t);
  }

  double ease_out_in_circ(double t)
  {
    return GenericEasing<double, circ>::ease_out_in(t);
  }

  double ease_in_back(double t)
  {
    return GenericEasing<double, back>::ease_in(t);
  }

  double ease_out_back(double t)
  {
    return GenericEasing<double, back>::ease_out(t);
  }

  double ease_in_out_back(double t)
  {
    return GenericEasing<double, back>::ease_in_out(t);
  }

  double ease_out_in_back(double t)
  {
    return GenericEasing<double, back>::ease_out_in(t);
  }

  double ease_in_bounce(double t)
  {
    return GenericEasing<double, bounce>::ease_in(t);
  }

  double ease_out_bounce(double t)
  {
    return GenericEasing<double, bounce>::ease_out(t);
  }

  double ease_in_out_bounce(double t)
  {
    return GenericEasing<double, bounce>::ease_in_out(t);
  }

  double ease_out_in_bounce(double t)
  {
    return GenericEasing<double, bounce>::ease_out_in(t);
  }

  double ease_in_elastic(double t)
  {
    return GenericEasing<double, elastic>::ease_in(t);
  }

  double ease_out_elastic(double t)
  {
    return GenericEasing<double, elastic>::ease_out(t);
  }

  double ease_in_out_elastic(double t)
  {
    return GenericEasing<double, elastic>::ease_in_out(t);
  }

  double ease_out_in_elastic(double t)
  {
    return GenericEasing<double, elastic>::ease_out_in(t);
  }

  double ease_in_expo(double t)
  {
    return GenericEasing<double, expo>::ease_in(t);
  }

  double ease_out_expo(double t)
  {
    return GenericEasing<double, expo>::ease_out(t);
  }

  double ease_in_out_expo(double t)
  {
    return GenericEasing<double, expo>::ease_in_out(t);
  }

  double ease_out_in_expo(double t)
  {
    return GenericEasing<double, expo>::ease_out_in(t);
  }

}
