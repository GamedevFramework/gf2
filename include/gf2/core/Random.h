// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_RANDOM_H
#define GF_RANDOM_H

#include <cstdint>

#include <limits>
#include <random>
#include <type_traits>

#include "Circ.h"
#include "CoreApi.h"
#include "Id.h"
#include "Rect.h"
#include "Vec2.h"

namespace gf {

  class GF_CORE_API RandomEngine {
  public:
    using result_type = uint64_t;

    RandomEngine();

    RandomEngine(result_type seed);

    static constexpr result_type min()
    {
      return std::numeric_limits<result_type>::min();
    }

    static constexpr result_type max()
    {
      return std::numeric_limits<result_type>::max();
    }

    result_type operator()();

    void short_jump();
    void long_jump();

  private:
    uint64_t next();

    uint64_t m_state[4];
  };

  class GF_CORE_API Random {
  public:
    Random();

    Random(uint64_t seed)
    : m_engine(seed)
    {
    }

    template<typename T>
    std::enable_if_t<std::is_integral_v<T>, T> compute_uniform_integer(T max)
    {
      assert(max > 0);
      return static_cast<T>(compute_raw_integer(max));
    }

    template<typename T>
    std::enable_if_t<std::is_integral_v<T>, T> compute_uniform_integer(T min, T max)
    {
      assert(min < max);
      using U = std::make_unsigned_t<T>;
      return min + static_cast<T>(compute_raw_integer(U(max) - U(min)));
    }

    template<typename T>
    std::enable_if_t<std::is_floating_point_v<T>, T> compute_uniform_float()
    {
      if constexpr (std::is_same_v<T, double>) {
        return compute_raw_double();
      } else if constexpr (std::is_same_v<T, float>) {
        return compute_raw_float();
      } else {
        return static_cast<T>(compute_raw_float());
      }
    }

    template<typename T>
    std::enable_if_t<std::is_floating_point_v<T>, T> compute_uniform_float(T max)
    {
      return compute_uniform_float<T>() * max;
    }

    template<typename T>
    std::enable_if_t<std::is_floating_point_v<T>, T> compute_uniform_float(T min, T max)
    {
      return min + compute_uniform_float<T>() * (max - min);
    }

    template<typename T>
    T compute_normal_float(T mean, T stddev)
    {
      std::normal_distribution<T> dist(mean, stddev);
      return dist(m_engine);
    }

    bool compute_bernoulli(double p)
    {
      std::bernoulli_distribution dist(p);
      return dist(m_engine);
    }

    Vec2F compute_position(const RectF& area);

    Vec2I compute_position(const RectI& area);

    Vec2F compute_position(const CircF& area);

    float compute_radius(float radius_min, float radius_max);

    float compute_angle();

    Id compute_id();

    RandomEngine& engine()
    {
      return m_engine;
    }

    uint64_t compute_raw_integer(uint64_t max);
    double compute_raw_double();
    float compute_raw_float();

  private:
    RandomEngine m_engine;
  };

} // namespace gf

#endif // GF_RANDOM_H
