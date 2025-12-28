// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_FLAGS_H
#define GF_FLAGS_H

#include <type_traits>

#include "Types.h"

namespace gf {
  class Serializer;
  class Deserializer;

  template<typename E>
  class Flags {
  public:
    static_assert(std::is_enum_v<E>, "E should be an enum");

    Flags() = default;

    constexpr Flags(NoneType /* unused */) noexcept
    : m_data(0)
    {
    }

    constexpr Flags(AllType /* unused */) noexcept
    : m_data(0)
    {
      m_data = ~m_data;
    }

    constexpr Flags(E e) noexcept
    : m_data(static_cast<Type>(e))
    {
    }

    constexpr Flags operator~() const noexcept
    {
      return Flags(~m_data);
    }

    constexpr Flags operator|(Flags flags) const noexcept
    {
      return Flags(m_data | flags.m_data);
    }

    constexpr Flags& operator|=(Flags flags) noexcept
    {
      m_data |= flags.m_data;
      return *this;
    }

    constexpr Flags operator&(Flags flags) const noexcept
    {
      return Flags(m_data & flags.m_data);
    }

    constexpr Flags& operator&=(Flags flags) noexcept
    {
      m_data &= flags.m_data;
      return *this;
    }

    constexpr operator bool() const noexcept
    {
      return m_data != 0;
    }

    constexpr bool test(E flag) const noexcept
    {
      return (m_data & static_cast<Type>(flag)) != 0;
    }

    constexpr void set(E flag) noexcept
    {
      m_data |= static_cast<Type>(flag);
    }

    constexpr void reset(E flag) noexcept
    {
      m_data &= ~static_cast<Type>(flag);
    }

    using Type = std::underlying_type_t<E>;

    constexpr Type value() const noexcept
    {
      return m_data;
    }

    template<typename T>
    friend Serializer& operator|(Serializer& ar, Flags<T> data);
    template<typename T>
    friend Deserializer& operator|(Deserializer& ar, Flags<T>& data);

  private:
    constexpr Flags(Type data)
    : m_data(data)
    {
    }

    Type m_data;
  };

  template<typename E>
  constexpr Flags<E> operator|(Flags<E> lhs, E rhs)
  {
    return lhs | Flags<E>(rhs);
  }

  template<typename E>
  constexpr Flags<E> operator|(E lhs, Flags<E> rhs)
  {
    return Flags<E>(lhs) | rhs;
  }

  template<typename E>
  constexpr Flags<E> operator&(Flags<E> lhs, E rhs)
  {
    return lhs & Flags<E>(rhs);
  }

  template<typename E>
  constexpr Flags<E> operator&(E lhs, Flags<E> rhs)
  {
    return Flags<E>(lhs) & rhs;
  }

  template<typename E>
  struct EnableBitmaskOperators : std::false_type {
  };

  namespace operators {

    template<typename E>
    constexpr Flags<E> operator|(E lhs, E rhs)
    requires EnableBitmaskOperators<E>::value
    {
      return Flags<E>(lhs) | Flags<E>(rhs);
    }

    template<typename E>
    constexpr Flags<E> operator&(E lhs, E rhs)
    requires EnableBitmaskOperators<E>::value
    {
      return Flags<E>(lhs) & Flags<E>(rhs);
    }

    template<typename E>
    constexpr Flags<E> operator~(E val)
    requires EnableBitmaskOperators<E>::value
    {
      return ~Flags<E>(val);
    }

  } // namespace operators
} // namespace gf

using namespace gf::operators;

#endif // GF_FLAGS_H
