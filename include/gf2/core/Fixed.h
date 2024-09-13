// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_FIXED_H
#define GF_FIXED_H

#include <cstdint>

#include <type_traits>

namespace gf {
  namespace details {

    template<typename T>
    struct LargerType;

    template<>
    struct LargerType<int8_t> { using type = int16_t; };
    template<>
    struct LargerType<int16_t> { using type = int32_t; };
    template<>
    struct LargerType<int32_t> { using type = int64_t; };
    template<>
    struct LargerType<uint8_t> { using type = uint16_t; };
    template<>
    struct LargerType<uint16_t> { using type = uint32_t; };
    template<>
    struct LargerType<uint32_t> { using type = uint64_t; };

    template<typename T>
    using Larger = typename LargerType<T>::type;

  }

  template<typename T, std::size_t Shift>
  class Fixed {
  public:
    static_assert(std::is_integral_v<T>);

    using Intermediary = details::Larger<T>;

    static constexpr Intermediary Factor = (Intermediary(1) << Shift);

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

    constexpr const T& raw() const
    {
      return m_raw;
    }

    constexpr T& raw()
    {
      return m_raw;
    }

    constexpr Fixed operator-() const
    {
      return { -m_raw, Raw };
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
      Intermediary raw = static_cast<Intermediary>(m_raw) * static_cast<Intermediary>(other.m_raw) / Factor;
      m_raw = static_cast<T>(raw);
      return *this;
    }

    constexpr Fixed& operator/=(Fixed other)
    {
      Intermediary raw = static_cast<Intermediary>(m_raw) * Factor / static_cast<Intermediary>(other.m_raw);
      m_raw = static_cast<T>(raw);
      return *this;
    }

  private:
    struct RawType {};
    static constexpr RawType Raw = {};

    constexpr Fixed(T raw, [[maybe_unused]] RawType tag)
    : m_raw(raw)
    {
    }

    T m_raw;
  };

  template<typename T, std::size_t Shift>
  constexpr Fixed<T, Shift> operator+(Fixed<T, Shift> lhs, Fixed<T, Shift> rhs)
  {
    return lhs += rhs;
  }

  template<typename T, std::size_t Shift, typename U>
  constexpr Fixed<T, Shift> operator+(Fixed<T, Shift> lhs, U rhs)
  {
    return lhs += Fixed<T, Shift>(rhs);
  }

  template<typename T, std::size_t Shift, typename U>
  constexpr Fixed<T, Shift> operator+(U lhs, Fixed<T, Shift> rhs)
  {
    return Fixed<T, Shift>(lhs) += rhs;
  }

  template<typename T, std::size_t Shift>
  constexpr Fixed<T, Shift> operator-(Fixed<T, Shift> lhs, Fixed<T, Shift> rhs)
  {
    return lhs -= rhs;
  }

  template<typename T, std::size_t Shift, typename U>
  constexpr Fixed<T, Shift> operator-(Fixed<T, Shift> lhs, U rhs)
  {
    return lhs -= Fixed<T, Shift>(rhs);
  }

  template<typename T, std::size_t Shift, typename U>
  constexpr Fixed<T, Shift> operator-(U lhs, Fixed<T, Shift> rhs)
  {
    return Fixed<T, Shift>(lhs) -= rhs;
  }

  template<typename T, std::size_t Shift>
  constexpr Fixed<T, Shift> operator*(Fixed<T, Shift> lhs, Fixed<T, Shift> rhs)
  {
    return lhs *= rhs;
  }

  template<typename T, std::size_t Shift, typename U>
  constexpr Fixed<T, Shift> operator*(Fixed<T, Shift> lhs, U rhs)
  {
    return lhs *= Fixed<T, Shift>(rhs);
  }

  template<typename T, std::size_t Shift, typename U>
  constexpr Fixed<T, Shift> operator*(U lhs, Fixed<T, Shift> rhs)
  {
    return Fixed<T, Shift>(lhs) *= rhs;
  }

  template<typename T, std::size_t Shift>
  constexpr Fixed<T, Shift> operator/(Fixed<T, Shift> lhs, Fixed<T, Shift> rhs)
  {
    return lhs /= rhs;
  }

  template<typename T, std::size_t Shift, typename U>
  constexpr Fixed<T, Shift> operator/(Fixed<T, Shift> lhs, U rhs)
  {
    return lhs /= Fixed<T, Shift>(rhs);
  }

  template<typename T, std::size_t Shift, typename U>
  constexpr Fixed<T, Shift> operator/(U lhs, Fixed<T, Shift> rhs)
  {
    return Fixed<T, Shift>(lhs) /= rhs;
  }

  template<typename T, std::size_t Shift, typename U>
  constexpr Fixed<T, Shift> operator%(Fixed<T, Shift> lhs, U rhs)
  {
    return lhs %= Fixed<T, Shift>(rhs);
  }

  template<typename T, std::size_t Shift, typename U>
  constexpr Fixed<T, Shift> operator%(U lhs, Fixed<T, Shift> rhs)
  {
    return Fixed<T, Shift>(lhs) %= rhs;
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

  namespace details {

    template<typename Archive, typename Self>
    Archive& handle_fixed_serialization(Archive& ar, Self& self)
    {
      return ar | self.raw();
    }

  }

  template<typename Archive, typename T, std::size_t Shift>
  inline Archive& operator|(Archive& ar, Fixed<T, Shift>& fixed)
  {
    return details::handle_fixed_serialization(ar, fixed);
  }

  template<typename Archive, typename T, std::size_t Shift>
  inline Archive& operator|(Archive& ar, const Fixed<T, Shift>& fixed)
  {
    return details::handle_fixed_serialization(ar, fixed);
  }

}

#endif // GF_FIXED_H
