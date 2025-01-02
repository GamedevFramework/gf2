// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_TAGGED_VARIANT_H
#define GF_TAGGED_VARIANT_H

#include <variant>

#include "TypeTraits.h"

namespace gf {

  template<typename Enum, typename ... Args>
  class TaggedVariant {
  public:
    TaggedVariant() = default;

    template<typename T, typename = PreventHiddenCopyMove<T, TaggedVariant>>
    constexpr TaggedVariant(T&& data)
    : m_data(std::forward<T>(data))
    {
    }

    Enum type() const
    {
      return static_cast<Enum>(m_data.index());
    }

    template<typename T>
    constexpr bool is() const
    {
      return std::holds_alternative<T>(m_data);
    }

    template<typename T>
    constexpr T& get()
    {
      return std::get<T>(m_data);
    }

    template<typename T>
    constexpr const T& get() const
    {
      return std::get<T>(m_data);
    }

    template<Enum E>
    constexpr auto& from()
    {
      return std::get<static_cast<std::size_t>(E)>(m_data);
    }

    template<Enum E>
    constexpr const auto& from() const
    {
      return std::get<static_cast<std::size_t>(E)>(m_data);
    }

    constexpr auto& raw()
    {
      return m_data;
    }

    constexpr const auto& raw() const
    {
      return m_data;
    }

  private:
    std::variant<std::monostate, Args...> m_data;
  };

  namespace details {

    template<typename Archive, typename Self>
    Archive& handle_tagged_variant_serialization(Archive& ar, Self& self)
    {
      return ar | self.raw();
    }

  }

  template<typename Archive, typename Enum, typename ... Args>
  inline Archive& operator|(Archive& ar, TaggedVariant<Enum, Args...>& variant)
  {
    return details::handle_tagged_variant_serialization(ar, variant);
  }

  template<typename Archive, typename Enum, typename ... Args>
  inline Archive& operator|(Archive& ar, const TaggedVariant<Enum, Args...>& variant)
  {
    return details::handle_tagged_variant_serialization(ar, variant);
  }

}

#endif // GF_TAGGED_VARIANT_H
