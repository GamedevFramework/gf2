// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PROPERTY_MAP_H
#define GF_PROPERTY_MAP_H

#include <cstdint>

#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "Color.h"
#include "CoreApi.h"
#include "Id.h"
#include "TypeTraits.h"

namespace gf {

  class Property;

  // NOLINTNEXTLINE(misc-no-recursion)
  class GF_CORE_API PropertyMap {
  public:

    void add_property(std::string key, bool value);
    void add_property(std::string key, int64_t value);
    void add_property(std::string key, double value);
    void add_property(std::string key, std::string value);
    void add_property(std::string key, std::filesystem::path value);
    void add_property(std::string key, Id value);
    void add_property(std::string key, Color value);
    void add_property(std::string key, PropertyMap value);

    bool has_property(std::string_view key) const;

    const Property& operator()(std::string_view path) const;

    bool empty() const
    {
      return m_properties.empty();
    }

  private:
    friend GF_CORE_API bool operator==(const PropertyMap& lhs, const PropertyMap& rhs);

    template<typename Archive>
    friend Archive& operator|(Archive& ar, MaybeConst<PropertyMap, Archive>& map);

    std::map<std::string, Property, std::less<void>> m_properties;
  };

  GF_CORE_API bool operator==(const PropertyMap& lhs, const PropertyMap& rhs);

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<PropertyMap, Archive>& map)
  {
    return ar | map.m_properties;
  }

}

#endif // GF_PROPERTY_MAP_H
