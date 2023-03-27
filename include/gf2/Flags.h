// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_FLAGS_H
#define GF_FLAGS_H

#include <type_traits>

#include "Types.h"

namespace gf {

  template<typename E>
  class Flags {
  public:
    static_assert(std::is_enum_v<E>, "E should be an enum");

    Flags() = default;

    constexpr Flags(NoneType /* unused */)
    : m_data(0)
    {
    }

    constexpr Flags(AllType /* unused */)
    : m_data(~Type(0))
    {
    }

    constexpr Flags(E e)
    : m_data(static_cast<Type>(e))
    {
    }

    constexpr Flags operator~() const
    {
      return Flags(~m_data);
    }

    constexpr Flags operator|(Flags flags) const
    {
      return Flags(m_data | flags.m_data);
    }

    Flags& operator|=(Flags flags)
    {
      m_data |= flags.m_data;
      return *this;
    }

    constexpr Flags operator&(Flags flags) const
    {
      return Flags(m_data & flags.m_data);
    }

    Flags& operator&=(Flags flags)
    {
      m_data &= flags.m_data;
      return *this;
    }

    constexpr operator bool() const
    {
      return m_data != 0;
    }

    constexpr bool test(E flag) const
    {
      return (m_data & static_cast<Type>(flag)) != 0;
    }

    void set(E flag)
    {
      m_data |= static_cast<Type>(flag);
    }

    void reset(E flag)
    {
      m_data &= ~static_cast<Type>(flag);
    }

    using Type = std::underlying_type_t<E>;

    constexpr Type value() const
    {
      return m_data;
    }

    //     template<typename T>
    //     friend Serializer& operator|(Serializer& ar, Flags<T> data);
    //     template<typename T>
    //     friend Deserializer& operator|(Deserializer& ar, Flags<T>& data);

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
    constexpr std::enable_if_t<EnableBitmaskOperators<E>::value, Flags<E>> operator|(E lhs, E rhs)
    {
      return Flags<E>(lhs) | Flags<E>(rhs);
    }

    template<typename E>
    constexpr std::enable_if_t<EnableBitmaskOperators<E>::value, Flags<E>> operator&(E lhs, E rhs)
    {
      return Flags<E>(lhs) & Flags<E>(rhs);
    }

    template<typename E>
    constexpr std::enable_if_t<EnableBitmaskOperators<E>::value, Flags<E>> operator~(E val)
    {
      return ~Flags<E>(val);
    }

  } // namespace operators
} // namespace gf

using namespace gf::operators;

#endif // GF_FLAGS_H
