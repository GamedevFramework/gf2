// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_RATIONAL_H
#define GF_RATIONAL_H

#include <numeric>
#include <stdexcept>
#include <type_traits>

namespace gf {

  template<typename T>
  class Rational {
  public:
    static_assert(std::is_integral_v<T>, "Rational only works with integer types.");

    constexpr Rational()
    : m_numerator(0)
    , m_denominator(1)
    {
    }

    constexpr Rational(T numerator)
    : m_numerator(numerator)
    , m_denominator(1)
    {
    }

    constexpr Rational(T numerator, T denominator)
    : m_numerator(numerator)
    , m_denominator(denominator)
    {
      normalize();
    }

    constexpr Rational& operator=(T integer)
    {
      m_numerator = integer;
      m_denominator = 1;
    }

    constexpr T numerator() const
    {
      return m_numerator;
    }

    constexpr T denominator() const
    {
      return m_denominator;
    }

    constexpr Rational& operator+=(const Rational& other)
    {
      const T numerator = other.m_numerator;
      const T denominator = other.m_denominator;

      const T g1 = std::gcd(denominator, m_denominator);
      m_denominator /= g1;
      m_numerator = m_numerator * (denominator / g1) + numerator * m_denominator;

      const T g2 = std::gcd(m_numerator, g1);
      m_numerator /= g2;
      m_denominator *= denominator / g2;

      return *this;
    }

    constexpr Rational& operator+=(T other)
    {
      m_numerator += m_denominator * other;
      return *this;
    }

    constexpr Rational& operator-=(const Rational& other)
    {
      const T numerator = other.m_numerator;
      const T denominator = other.m_denominator;

      const T g1 = std::gcd(denominator, m_denominator);
      m_denominator /= g1;
      m_numerator = m_numerator * (denominator / g1) - numerator * m_denominator;

      const T g2 = std::gcd(m_numerator, g1);
      m_numerator /= g2;
      m_denominator *= denominator / g2;

      return *this;
    }

    constexpr Rational& operator-=(T other)
    {
      m_numerator -= m_denominator * other;
      return *this;
    }

    constexpr Rational& operator*=(const Rational& other)
    {
      const T numerator = other.m_numerator;
      const T denominator = other.m_denominator;

      const T g1 = std::gcd(m_numerator, denominator);
      const T g2 = std::gcd(numerator, m_denominator);

      m_numerator = (m_numerator / g1) * (numerator / g2);
      m_denominator = (m_denominator / g2) * (denominator / g1);

      return *this;
    }

    constexpr Rational& operator*=(T other)
    {
      const T gcd = std::gcd(other, m_denominator);
      m_numerator *= other / gcd;
      m_denominator /= gcd;
      return *this;
    }

    constexpr Rational& operator/=(const Rational& other)
    {
      const T numerator = other.m_numerator;
      const T denominator = other.m_denominator;

      if (numerator == T(0)) {
        throw std::runtime_error("Division by zero");
      }

      if (m_numerator == T(0)) {
        return *this;
      }

      const T g1 = std::gcd(m_numerator, numerator);
      const T g2 = std::gcd(denominator, m_denominator);

      m_numerator = (m_numerator / g1) * (denominator / g2);
      m_denominator = (m_denominator / g2) * (numerator / g1);

      if (m_denominator < T(0)) {
        m_denominator = -m_denominator;
        m_numerator = -m_numerator;
      }

      return *this;
    }

    constexpr Rational& operator/=(T other)
    {
      if (other == T(0)) {
        throw std::runtime_error("Division by zero");
      }

      if (m_numerator == T(0)) {
        return *this;
      }

      const T gcd = std::gcd(m_numerator, other);
      m_numerator /= gcd;
      m_denominator *= other / gcd;

      if (m_denominator < T(0)) {
        m_denominator = -m_denominator;
        m_numerator = -m_numerator;
      }

      return *this;
    }

  private:
    constexpr void normalize()
    {
      if (m_denominator == 0) {
        throw std::runtime_error("Division by zero");
      }

      if (m_numerator == 0) {
        m_denominator = 1;
        return;
      }

      auto gcd = std::gcd(m_numerator, m_denominator);
      m_numerator /= gcd;
      m_denominator /= gcd;

      if (m_denominator < 0) {
        m_denominator = -m_denominator;
        m_numerator = -m_numerator;
      }
    }

    T m_numerator = 0;
    T m_denominator = 1;
  };

  template<typename T>
  constexpr Rational<T> operator+(const Rational<T>& rhs)
  {
    return rhs;
  }

  template<typename T>
  constexpr Rational<T> operator-(const Rational<T>& rhs)
  {
    return { -rhs.numerator(), rhs.denominator() };
  }

  template<typename T>
  constexpr Rational<T> operator+(const Rational<T>& lhs, const Rational<T>& rhs)
  {
    Rational<T> tmp(lhs);
    return tmp += rhs;
  }

  template<typename T>
  constexpr Rational<T> operator+(const Rational<T>& lhs, T rhs)
  {
    Rational<T> tmp(lhs);
    return tmp += rhs;
  }

  template<typename T>
  constexpr Rational<T> operator+(T lhs, const Rational<T>& rhs)
  {
    Rational<T> tmp(rhs);
    return tmp += lhs;
  }

  template<typename T>
  constexpr Rational<T> operator-(const Rational<T>& lhs, const Rational<T>& rhs)
  {
    Rational<T> tmp(lhs);
    return tmp -= rhs;
  }

  template<typename T>
  constexpr Rational<T> operator-(const Rational<T>& lhs, T rhs)
  {
    Rational<T> tmp(lhs);
    return tmp -= rhs;
  }

  template<typename T>
  constexpr Rational<T> operator-(T lhs, const Rational<T>& rhs)
  {
    Rational<T> tmp(rhs);
    return -(tmp -= lhs);
  }

  template<typename T>
  constexpr Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs)
  {
    Rational<T> tmp(lhs);
    return tmp *= rhs;
  }

  template<typename T>
  constexpr Rational<T> operator*(const Rational<T>& lhs, T rhs)
  {
    Rational<T> tmp(lhs);
    return tmp *= rhs;
  }

  template<typename T>
  constexpr Rational<T> operator*(T lhs, const Rational<T>& rhs)
  {
    Rational<T> tmp(rhs);
    return tmp *= lhs;
  }

  template<typename T>
  constexpr Rational<T> operator/(const Rational<T>& lhs, const Rational<T>& rhs)
  {
    Rational<T> tmp(lhs);
    return tmp /= rhs;
  }

  template<typename T>
  constexpr Rational<T> operator/(const Rational<T>& lhs, T rhs)
  {
    Rational<T> tmp(lhs);
    return tmp /= rhs;
  }

  template<typename T>
  constexpr Rational<T> operator/(T lhs, const Rational<T>& rhs)
  {
    Rational<T> tmp(lhs);
    return tmp /= rhs;
  }

  template<typename T>
  constexpr bool operator==(const Rational<T>& lhs, const Rational<T>& rhs)
  {
    return lhs.numerator() == rhs.numerator() && lhs.denominator() == rhs.denominator();
  }

  template<typename T>
  constexpr bool operator==(const Rational<T>& lhs, T rhs)
  {
    return lhs.numerator() == rhs && lhs.denominator() == 1;
  }

  template<typename T>
  constexpr bool operator==(T lhs, const Rational<T>& rhs)
  {
    return rhs == lhs;
  }

  template<typename T>
  constexpr bool operator!=(const Rational<T>& lhs, const Rational<T>& rhs)
  {
    return !(lhs == rhs);
  }

  template<typename T>
  constexpr bool operator!=(const Rational<T>& lhs, T rhs)
  {
    return !(lhs == rhs);
  }

  template<typename T>
  constexpr bool operator!=(T lhs, const Rational<T>& rhs)
  {
    return !(lhs == rhs);
  }

  template<typename T>
  constexpr bool operator<(const Rational<T>& lhs, const Rational<T>& rhs)
  {
    struct Euclid {
      T n;
      T d;
      T q;
      T r;

      void normalize() {
        while (r < T(0)) {
          r += d;
          --q;
        }
      }

      void step() {
        n = r;
        d = r;
        q = n / d;
        r = n % d;
      }
    };

    Euclid le = { lhs.numerator(), lhs.denominator(), lhs.numerator() / lhs.denominator(), lhs.numerator() % lhs.denominator() };
    Euclid re = { rhs.numerator(), rhs.denominator(), rhs.numerator() / rhs.denominator(), rhs.numerator() % rhs.denominator() };
    bool reverse = false;

    le.normalize();
    re.normalize();

    for (;;) {
      if (le.q != re.q) {
        return reverse ? le.q > re.q : le.q < re.q;
      }

      reverse = !reverse;

      if (le.r == 0 || re.r == 0) {
        break;
      }

      le.step();
      re.step();
    }

    if (le.r == re.r) {
      return false;
    }

    return (le.r != T(0)) != reverse;
  }

  template<typename T>
  constexpr bool operator<(const Rational<T>& lhs, T rhs)
  {
    T q = lhs.numerator() / lhs.denominator();
    T r = lhs.numerator() % lhs.denominator();

    while (r < T(0)) {
      r += lhs.denominator();
      --q;
    }

    return q < rhs;
  }

  template<typename T>
  constexpr bool operator<(T lhs, const Rational<T>& rhs)
  {
    if (lhs == rhs) {
      return false;
    }

    return !(rhs < lhs);
  }

  template<typename T>
  constexpr bool operator>(const Rational<T>& lhs, const Rational<T>& rhs)
  {
    return rhs < lhs;
  }

  template<typename T>
  constexpr bool operator>(const Rational<T>& lhs, T rhs)
  {
    if (lhs == rhs) {
      return false;
    }

    return !(lhs < rhs);
  }

  template<typename T>
  constexpr bool operator>(T lhs, const Rational<T>& rhs)
  {
    return rhs < lhs;
  }

  template<typename T>
  constexpr bool operator<=(const Rational<T>& lhs, const Rational<T>& rhs)
  {
    return !(rhs < lhs);
  }

  template<typename T>
  constexpr bool operator<=(const Rational<T>& lhs, T rhs)
  {
    return !(rhs < lhs);
  }

  template<typename T>
  constexpr bool operator<=(T lhs, const Rational<T>& rhs)
  {
    return !(rhs < lhs);
  }

  template<typename T>
  constexpr bool operator>=(const Rational<T>& lhs, const Rational<T>& rhs)
  {
    return !(lhs < rhs);
  }

  template<typename T>
  constexpr bool operator>=(const Rational<T>& lhs, T rhs)
  {
    return !(lhs < rhs);
  }

  template<typename T>
  constexpr bool operator>=(T lhs, const Rational<T>& rhs)
  {
    return !(lhs < rhs);
  }
}

#endif // GF_RATIONAL_H
