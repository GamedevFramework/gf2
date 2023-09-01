// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PROPERTY_MAP_H
#define GF_PROPERTY_MAP_H

#include <map>
#include <string>
#include <string_view>

#include "CoreApi.h"

namespace gf {

  class Property;

  class GF_CORE_API PropertyMap {
  public:

    void add_property(std::string key, Property value);

    const Property& operator()(std::string_view path) const;

  private:
    friend bool operator==(const PropertyMap& lhs, const PropertyMap& rhs);

    std::map<std::string, Property, std::less<void>> m_properties;
  };

  GF_CORE_API bool operator==(const PropertyMap& lhs, const PropertyMap& rhs);

}

#endif // GF_PROPERTY_MAP_H
