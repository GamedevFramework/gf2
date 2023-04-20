// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Random.h>
// clang-format on

#include <cinttypes>

#include <gf2/Math.h>

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
    std::uniform_real_distribution<float> dist_x(area.offset.x, area.offset.x + area.extent.x);
    std::uniform_real_distribution<float> dist_y(area.offset.y, area.offset.y + area.extent.y);
    const float x = dist_x(m_engine);
    const float y = dist_y(m_engine);
    return { x, y };
  }

  Vec2I Random::compute_position(const RectI& area)
  {
    std::uniform_int_distribution<int> dist_x(area.offset.x, area.offset.x + area.extent.x);
    std::uniform_int_distribution<int> dist_y(area.offset.y, area.offset.y + area.extent.y);
    const int x = dist_x(m_engine);
    const int y = dist_y(m_engine);
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
    std::uniform_real_distribution<float> dist(gf::square(radius_min), gf::square(radius_max));
    return std::sqrt(dist(m_engine));
  }

  float Random::compute_angle()
  {
    std::uniform_real_distribution<float> dist(0.0f, 2.0f * gf::Pi);
    return dist(m_engine);
  }

  Id Random::compute_id()
  {
    using Type = std::underlying_type_t<Id>;
    const Type lo = std::numeric_limits<Type>::min() + 1;
    const Type hi = std::numeric_limits<Type>::max();
    return static_cast<Id>(compute_uniform_integer(lo, hi));
  }

} // namespace gf
