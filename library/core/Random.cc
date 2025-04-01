// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Random.h>

#include <gf2/core/Math.h>

namespace gf {

  namespace {

    constexpr uint64_t rotl(const uint64_t x, int k)
    {
      return (x << k) | (x >> (64 - k));
    }

    // https://prng.di.unimi.it/splitmix64.c
    class SplitMix64 {
    public:
      constexpr SplitMix64(uint64_t seed)
      : m_state(seed)
      {
      }

      uint64_t next()
      {
        uint64_t z = (m_state += UINT64_C(0x9e3779b97f4a7c15));
        z = (z ^ (z >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
        z = (z ^ (z >> 27)) * UINT64_C(0x94d049bb133111eb);
        return z ^ (z >> 31);
      }

    private:
      uint64_t m_state;
    };

    uint64_t compute_random_seed()
    {
      std::random_device rd;
      uint64_t result = rd();
      result = (result << 32) + rd();
      return result;
    }

  } // anonymous namespace

  // RandomEngine

  RandomEngine::RandomEngine()
  : m_state{ 0, 0, 0, 0 }
  {
    for (auto& state : m_state) {
      state = compute_random_seed();
    }
  }

  RandomEngine::RandomEngine(result_type seed)
  : m_state{ 0, 0, 0, 0 }
  {
    SplitMix64 sm(seed);

    for (auto& state : m_state) {
      state = sm.next();
    }
  }

  auto RandomEngine::operator()() -> result_type
  {
    return next();
  }

  // https://prng.di.unimi.it/xoshiro256plusplus.c
  uint64_t RandomEngine::next()
  {
    const uint64_t result = rotl(m_state[0] + m_state[3], 23) + m_state[0];
    const uint64_t t = m_state[1] << 17;

    m_state[2] ^= m_state[0];
    m_state[3] ^= m_state[1];
    m_state[1] ^= m_state[2];
    m_state[0] ^= m_state[3];

    m_state[2] ^= t;

    m_state[3] = rotl(m_state[3], 45);

    return result;
  }

  void RandomEngine::short_jump()
  {
    static constexpr uint64_t Jump[] = {
      UINT64_C(0x180ec6d33cfd0aba),
      UINT64_C(0xd5a61266f0c9392c),
      UINT64_C(0xa9582618e03fc9aa),
      UINT64_C(0x39abdc4529b1661c)
    };

    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;

    for (auto jump : Jump) {
      for (int b = 0; b < 64; b++) {
        if ((jump & UINT64_C(1) << b) != 0) {
          s0 ^= m_state[0];
          s1 ^= m_state[1];
          s2 ^= m_state[2];
          s3 ^= m_state[3];
        }

        next();
      }
    }

    m_state[0] = s0;
    m_state[1] = s1;
    m_state[2] = s2;
    m_state[3] = s3;
  }

  void RandomEngine::long_jump()
  {
    static constexpr uint64_t Jump[] = {
      UINT64_C(0x76e15d3efefdcbbf),
      UINT64_C(0xc5004e441c522fb3),
      UINT64_C(0x77710069854ee241),
      UINT64_C(0x39109bb02acbe635)
    };

    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;

    for (auto jump : Jump) {
      for (int b = 0; b < 64; b++) {
        if ((jump & UINT64_C(1) << b) != 0) {
          s0 ^= m_state[0];
          s1 ^= m_state[1];
          s2 ^= m_state[2];
          s3 ^= m_state[3];
        }

        next();
      }
    }

    m_state[0] = s0;
    m_state[1] = s1;
    m_state[2] = s2;
    m_state[3] = s3;
  }

  // Random

  Random::Random()
  {
    m_engine.short_jump();
  }

  Vec2F Random::compute_position(const RectF& area)
  {
    const float x = compute_uniform_float(area.offset.x, area.offset.x + area.extent.w);
    const float y = compute_uniform_float(area.offset.y, area.offset.y + area.extent.h);
    return { x, y };
  }

  Vec2I Random::compute_position(const RectI& area)
  {
    const int x = compute_uniform_integer(area.offset.x, area.offset.x + area.extent.w);
    const int y = compute_uniform_integer(area.offset.y, area.offset.y + area.extent.h);
    return { x, y };
  }

  Vec2F Random::compute_position(const CircF& area)
  {
    const float angle = compute_angle();
    const float radius = compute_radius(0.0f, area.radius);
    return area.center + radius * gf::unit(angle);
  }

  float Random::compute_radius(float radius_min, float radius_max)
  {
    return std::sqrt(compute_uniform_float(gf::square(radius_min), gf::square(radius_max)));
  }

  float Random::compute_angle()
  {
    return compute_uniform_float(2.0f * gf::Pi);
  }

  Id Random::compute_id()
  {
    uint64_t id = m_engine();

    while (id == 0) {
      id = m_engine();
    }

    return Id{ id };
  }

  // using "Debiased Modulo (Once) — Java's Method" from
  // https://www.pcg-random.org/posts/bounded-rands.html
  uint64_t Random::compute_raw_integer(uint64_t max)
  {
    assert(max > 0);

    uint64_t x = 0;
    uint64_t r = 0;
    const uint64_t threshold = std::numeric_limits<uint64_t>::max() - max + 1;

    do { // NOLINT(cppcoreguidelines-avoid-do-while)
      x = m_engine();
      r = x % max;
    } while (x - r > threshold);

    return r;
  }

  double Random::compute_raw_float()
  {
    const uint64_t value = m_engine();
    return double(value >> 11) * 0x1.0p-53;
  }

} // namespace gf
