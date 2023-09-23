// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_FIXED_H
#define GF_FIXED_H

#include <cstdint>

#include <type_traits>

namespace gf {

  template<typename T, std::size_t Shift>
  class Fixed {
  public:
    static_assert(std::is_integral_v<T>);

    static constexpr T Factor = (T(1) << Shift);

    constexpr Fixed()
    : m_raw(0)
    {
    }

    constexpr Fixed(T val)
    : m_raw(val * Factor)
    {
    }

    constexpr Fixed(float val)
    : m_raw(static_cast<T>(val * Factor))
    {
    }

    constexpr T as_int() const
    {
      return m_raw / Factor;
    }

    constexpr float as_float() const
    {
      return static_cast<float>(m_raw) / static_cast<float>(Factor);
    }

    constexpr T raw() const
    {
      return m_raw;
    }

    constexpr Fixed& operator+=(Fixed other)
    {
      m_raw += other.m_raw;
      return *this;
    }

    constexpr Fixed& operator-=(Fixed other)
    {
      m_raw -= other.m_raw;
      return *this;
    }

    constexpr Fixed& operator*=(Fixed other)
    {
      m_raw *= other.m_raw;
      m_raw /= Factor;
      return *this;
    }

    constexpr Fixed& operator/=(Fixed other)
    {
      m_raw *= Factor;
      m_raw /= other.m_raw;
      return *this;
    }

  private:
    T m_raw;
  };

  template<typename T, std::size_t Shift>
  constexpr Fixed<T, Shift> operator+(Fixed<T, Shift> lhs, Fixed<T, Shift> rhs)
  {
    return lhs += rhs;
  }

  template<typename T, std::size_t Shift>
  constexpr Fixed<T, Shift> operator-(Fixed<T, Shift> lhs, Fixed<T, Shift> rhs)
  {
    return lhs -= rhs;
  }

  template<typename T, std::size_t Shift>
  constexpr Fixed<T, Shift> operator*(Fixed<T, Shift> lhs, Fixed<T, Shift> rhs)
  {
    return lhs *= rhs;
  }

  template<typename T, std::size_t Shift>
  constexpr Fixed<T, Shift> operator/(Fixed<T, Shift> lhs, Fixed<T, Shift> rhs)
  {
    return lhs /= rhs;
  }

  template<typename T, std::size_t Shift>
  constexpr Fixed<T, Shift> operator%(Fixed<T, Shift> lhs, Fixed<T, Shift> rhs)
  {
    return lhs %= rhs;
  }

  template<typename T, std::size_t Shift>
  constexpr bool operator==(Fixed<T, Shift> lhs, Fixed<T, Shift> rhs)
  {
    return lhs.raw() == rhs.raw();
  }

  template<typename T, std::size_t Shift>
  constexpr bool operator!=(Fixed<T, Shift> lhs, Fixed<T, Shift> rhs)
  {
    return lhs.raw() != rhs.raw();
  }

  template<typename T, std::size_t Shift>
  constexpr bool operator<(Fixed<T, Shift> lhs, Fixed<T, Shift> rhs)
  {
    return lhs.raw() < rhs.raw();
  }

  template<typename T, std::size_t Shift>
  constexpr bool operator<=(Fixed<T, Shift> lhs, Fixed<T, Shift> rhs)
  {
    return lhs.raw() <= rhs.raw();
  }

  template<typename T, std::size_t Shift>
  constexpr bool operator>(Fixed<T, Shift> lhs, Fixed<T, Shift> rhs)
  {
    return lhs.raw() > rhs.raw();
  }

  template<typename T, std::size_t Shift>
  constexpr bool operator>=(Fixed<T, Shift> lhs, Fixed<T, Shift> rhs)
  {
    return lhs.raw() >= rhs.raw();
  }

  using Fixed32 = Fixed<int32_t, 16>;

}

#endif // GF_FIXED_H
