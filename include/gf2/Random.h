// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_RANDOM_H
#define GF_RANDOM_H

#include <cstdint>

#include <limits>
#include <random>

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
    T compute_uniform_integer(T min, T max)
    {
      std::uniform_int_distribution<T> dist(min, max);
      return dist(m_engine);
    }

    template<typename T>
    T compute_uniform_float(T min, T max)
    {
      std::uniform_real_distribution<T> dist(min, max);
      return dist(m_engine);
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

  private:
    RandomEngine m_engine;
  };

} // namespace gf

#endif // GF_RANDOM_H
