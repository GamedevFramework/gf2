// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PROPERTY_H
#define GF_PROPERTY_H

#include <cassert>

#include <filesystem>
#include <string>
#include <variant>

#include "Color.h"
#include "CoreApi.h"
#include "Id.h"
#include "PropertyMap.h"
#include "TypeTraits.h"

namespace gf {

  // NOLINTNEXTLINE(misc-no-recursion)
  class GF_CORE_API Property {
  public:
    Property() = default;

    template<typename T, typename = PreventHiddenCopyMove<T, Property>>
    explicit Property(T&& value)
    : m_value(std::forward<T>(value))
    {
    }

    bool is_bool() const
    {
      return is<bool>();
    }

    bool is_int() const
    {
      return is<int64_t>();
    }

    bool is_float() const
    {
      return is<double>();
    }

    bool is_string() const
    {
      return is<std::string>();
    }

    bool is_path() const
    {
      return is<std::filesystem::path>();
    }

    bool is_object() const
    {
      return is<Id>();
    }

    bool is_color() const
    {
      return is<Color>();
    }

    bool is_class() const
    {
      return is<PropertyMap>();
    }

    bool as_bool() const
    {
      return as<bool>();
    }

    int64_t as_int() const
    {
      return as<int64_t>();
    }

    double as_float() const
    {
      return as<double>();
    }

    const std::string& as_string() const
    {
      return as<std::string>();
    }

    const std::filesystem::path& as_path() const
    {
      return as<std::filesystem::path>();
    }

    Id as_object() const
    {
      return as<Id>();
    }

    Color as_color() const
    {
      return as<Color>();
    }

    const PropertyMap& as_class() const
    {
      return as<PropertyMap>();
    }

  private:
    friend inline bool operator==(const Property& lhs, const Property& rhs);

    template<typename Archive>
    friend Archive& operator|(Archive& ar, MaybeConst<Property, Archive>& property);

    template<typename T>
    bool is() const
    {
      return std::holds_alternative<T>(m_value);
    }

    template<typename T>
    const T& as() const
    {
      assert(std::holds_alternative<T>(m_value));
      return std::get<T>(m_value);
    }

    std::variant<std::monostate, bool, int64_t, double, std::string, std::filesystem::path, Id, Color, PropertyMap> m_value;
  };

  // NOLINTNEXTLINE(misc-no-recursion)
  inline bool operator==(const Property& lhs, const Property& rhs)
  {
    return lhs.m_value == rhs.m_value; // NOLINT(misc-no-recursion)
  }

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<Property, Archive>& property)
  {
    return ar | property.m_value;
  }

}

#endif // GF_PROPERTY_H
