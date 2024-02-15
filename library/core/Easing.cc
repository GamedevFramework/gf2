// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/Easing.h>

#include <cmath>

#include <gf2/core/Math.h>

namespace gf {

  namespace {

    template<Easing Func>
    class GenericEasing {
    public:
      static float ease_in(float t)
      {
        return Func(t);
      }

      static float ease_out(float t)
      {
        return 1 - Func(1 - t);
      }

      static float ease_in_out(float t)
      {
        return t < 0.5f ? ease_in(2 * t) / 2 : 0.5f + ease_out(2 * t - 1) / 2;
      }

      static float ease_out_in(float t)
      {
        return t < 0.5f ? ease_out(2 * t) / 2 : 0.5f + ease_in(2 * t - 1) / 2;
      }
    };

    constexpr float quad(float t)
    {
      return t * t;
    }

    constexpr float cubic(float t)
    {
      return t * t * t;
    }

    constexpr float quart(float t)
    {
      return t * t * t * t;
    }

    constexpr float quint(float t)
    {
      return t * t * t * t * t;
    }

    inline float circ(float t)
    {
      return 1 - std::sqrt(1 - t * t);
    }

    inline float sine(float t)
    {
      return (1 - std::cos(Pi2 * t));
    }

    inline float back(float t)
    {
      static constexpr float C1 = 1.70158f;
      static constexpr float C2 = 1 + C1;
      return t * t * (C2 * t - C1);
    }

    inline float bounce(float t)
    {
      static constexpr float C1 = 1.0f / 2.75f;
      static constexpr float C2 = 7.5625f; // = 2.75f * 2.75f

      const float u = 1 - t;

      if (u < 1 * C1) {
        return 1 - C2 * u * u;
      }

      if (u < 2 * C1) {
        return 1 - (C2 * gf::square(u - 1.5f * C1) + 0.75f);
      }

      if (u < 2.5f * C1) {
        return 1 - (C2 * gf::square(u - 2.25f * C1) + 0.9375f);
      }

      return 1 - (C2 * gf::square(u - 2.625f * C1) + 0.984375f);
    }

    inline float elastic(float t)
    {
      static constexpr float C0 = 0.3f;

      const float u = t - 1;

      return -std::pow(2.0f, 10 * u) * std::sin((u - C0 / 4) * 2 * Pi / C0);
    }

    inline float expo(float t)
    {
      return t == 0.0f ? 0.0f : std::pow(2.0f, 10 * (t - 1));
    }

  } // anonymous namespace

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
    return GenericEasing<sine>::ease_in(t);
  }

  float ease_out_sine(float t)
  {
    return GenericEasing<sine>::ease_out(t);
  }

  float ease_in_out_sine(float t)
  {
    return GenericEasing<sine>::ease_in_out(t);
  }

  float ease_out_in_sine(float t)
  {
    return GenericEasing<sine>::ease_out_in(t);
  }

  float ease_in_quad(float t)
  {
    return GenericEasing<quad>::ease_in(t);
  }

  float ease_out_quad(float t)
  {
    return GenericEasing<quad>::ease_out(t);
  }

  float ease_in_out_quad(float t)
  {
    return GenericEasing<quad>::ease_in_out(t);
  }

  float ease_out_in_quad(float t)
  {
    return GenericEasing<quad>::ease_out_in(t);
  }

  float ease_in_cubic(float t)
  {
    return GenericEasing<cubic>::ease_in(t);
  }

  float ease_out_cubic(float t)
  {
    return GenericEasing<cubic>::ease_out(t);
  }

  float ease_in_out_cubic(float t)
  {
    return GenericEasing<cubic>::ease_in_out(t);
  }

  float ease_out_in_cubic(float t)
  {
    return GenericEasing<cubic>::ease_out_in(t);
  }

  float ease_in_quart(float t)
  {
    return GenericEasing<quart>::ease_in(t);
  }

  float ease_out_quart(float t)
  {
    return GenericEasing<quart>::ease_out(t);
  }

  float ease_in_out_quart(float t)
  {
    return GenericEasing<quart>::ease_in_out(t);
  }

  float ease_out_in_quart(float t)
  {
    return GenericEasing<quart>::ease_out_in(t);
  }

  float ease_in_quint(float t)
  {
    return GenericEasing<quint>::ease_in(t);
  }

  float ease_out_quint(float t)
  {
    return GenericEasing<quint>::ease_out(t);
  }

  float ease_in_out_quint(float t)
  {
    return GenericEasing<quint>::ease_in_out(t);
  }

  float ease_out_in_quint(float t)
  {
    return GenericEasing<quint>::ease_out_in(t);
  }

  float ease_in_circ(float t)
  {
    return GenericEasing<circ>::ease_in(t);
  }

  float ease_out_circ(float t)
  {
    return GenericEasing<circ>::ease_out(t);
  }

  float ease_in_out_circ(float t)
  {
    return GenericEasing<circ>::ease_in_out(t);
  }

  float ease_out_in_circ(float t)
  {
    return GenericEasing<circ>::ease_out_in(t);
  }

  float ease_in_back(float t)
  {
    return GenericEasing<back>::ease_in(t);
  }

  float ease_out_back(float t)
  {
    return GenericEasing<back>::ease_out(t);
  }

  float ease_in_out_back(float t)
  {
    return GenericEasing<back>::ease_in_out(t);
  }

  float ease_out_in_back(float t)
  {
    return GenericEasing<back>::ease_out_in(t);
  }

  float ease_in_bounce(float t)
  {
    return GenericEasing<bounce>::ease_in(t);
  }

  float ease_out_bounce(float t)
  {
    return GenericEasing<bounce>::ease_out(t);
  }

  float ease_in_out_bounce(float t)
  {
    return GenericEasing<bounce>::ease_in_out(t);
  }

  float ease_out_in_bounce(float t)
  {
    return GenericEasing<bounce>::ease_out_in(t);
  }

  float ease_in_elastic(float t)
  {
    return GenericEasing<elastic>::ease_in(t);
  }

  float ease_out_elastic(float t)
  {
    return GenericEasing<elastic>::ease_out(t);
  }

  float ease_in_out_elastic(float t)
  {
    return GenericEasing<elastic>::ease_in_out(t);
  }

  float ease_out_in_elastic(float t)
  {
    return GenericEasing<elastic>::ease_out_in(t);
  }

  float ease_in_expo(float t)
  {
    return GenericEasing<expo>::ease_in(t);
  }

  float ease_out_expo(float t)
  {
    return GenericEasing<expo>::ease_out(t);
  }

  float ease_in_out_expo(float t)
  {
    return GenericEasing<expo>::ease_in_out(t);
  }

  float ease_out_in_expo(float t)
  {
    return GenericEasing<expo>::ease_out_in(t);
  }

}
