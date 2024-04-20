// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TIME_H
#define GF_TIME_H

#include <cstdint>

#include <chrono>
#include <ratio>

#include "CoreApi.h"

namespace gf {
  class Serializer;
  class Deserializer;

  namespace details {
    using ClockType = std::conditional_t<std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock, std::chrono::steady_clock>;
    using DurationType = ClockType::duration;
    using TimePointType = ClockType::time_point;
  }

  class GF_CORE_API Time {
  public:

    constexpr Time() = default;

    constexpr explicit Time(details::DurationType duration)
    : m_duration(duration)
    {
    }

    constexpr float as_seconds() const
    {
      return std::chrono::duration_cast<std::chrono::duration<float>>(m_duration).count();
    }

    constexpr int32_t as_milliseconds() const
    {
      return std::chrono::duration_cast<std::chrono::duration<int32_t, std::milli>>(m_duration).count();
    }

    constexpr int64_t as_microseconds() const
    {
      return std::chrono::duration_cast<std::chrono::duration<int64_t, std::micro>>(m_duration).count();
    }

    constexpr std::chrono::steady_clock::duration as_duration() const
    {
      return m_duration;
    }

    Time& operator+=(Time other)
    {
      m_duration += other.m_duration;
      return *this;
    }

    Time& operator-=(Time other)
    {
      m_duration -= other.m_duration;
      return *this;
    }

  private:
    details::DurationType m_duration = details::DurationType::zero();
  };

  constexpr Time seconds(float amount)
  {
    return Time(std::chrono::duration_cast<details::DurationType>(std::chrono::duration<float>(amount)));
  }

  constexpr Time milliseconds(int32_t amount)
  {
    return Time(std::chrono::duration_cast<details::DurationType>(std::chrono::duration<int32_t, std::milli>(amount)));
  }

  constexpr Time microseconds(int64_t amount)
  {
    return Time(std::chrono::duration_cast<details::DurationType>(std::chrono::duration<int64_t, std::micro>(amount)));
  }

  namespace literals {

    constexpr Time operator""_seconds(long double amount)
    {
      return seconds(static_cast<float>(amount));
    }

    constexpr Time operator""_milliseconds(unsigned long long int amount)
    {
      return milliseconds(static_cast<int32_t>(amount));
    }

    constexpr Time operator""_microseconds(unsigned long long int amount)
    {
      return microseconds(static_cast<int64_t>(amount));
    }

  } // namespace literals

  constexpr bool operator==(Time lhs, Time rhs)
  {
    return lhs.as_duration() == rhs.as_duration();
  }

  constexpr bool operator!=(Time lhs, Time rhs)
  {
    return lhs.as_duration() != rhs.as_duration();
  }

  constexpr bool operator<(Time lhs, Time rhs)
  {
    return lhs.as_duration() < rhs.as_duration();
  }

  constexpr bool operator>(Time lhs, Time rhs)
  {
    return lhs.as_duration() > rhs.as_duration();
  }

  constexpr bool operator<=(Time lhs, Time rhs)
  {
    return lhs.as_duration() <= rhs.as_duration();
  }

  constexpr bool operator>=(Time lhs, Time rhs)
  {
    return lhs.as_duration() >= rhs.as_duration();
  }

  constexpr Time operator+(Time lhs, Time rhs)
  {
    return Time(lhs.as_duration() + rhs.as_duration());
  }

  constexpr Time operator-(Time lhs, Time rhs)
  {
    return Time(lhs.as_duration() - rhs.as_duration());
  }

  GF_CORE_API Serializer& operator|(Serializer& archive, Time time);
  GF_CORE_API Deserializer& operator|(Deserializer& archive, Time& time);

} // namespace gf

#endif // GF_TIME_H
