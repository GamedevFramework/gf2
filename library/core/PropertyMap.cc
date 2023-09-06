// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/core/PropertyMap.h>
// clang-format on

#include <cassert>

#include <stdexcept>

#include <gf2/core/Log.h>
#include <gf2/core/Property.h>
#include <gf2/core/StringUtils.h>

namespace gf {

  void PropertyMap::add_property(std::string key, Property value)
  {
    m_properties.emplace(std::move(key), std::move(value));
  }

  const Property& PropertyMap::operator()(std::string_view path) const
  {
    auto keys = split_path(path);

    if (keys.empty()) {
      Log::error("Failed to get a property, empty path.");
      throw std::runtime_error("Failed to get a property, empty path.");
    }

    std::string_view last_key = keys.back();
    keys.pop_back();

    const PropertyMap* current_map = this;

    for (auto key : keys) {
      const auto& properties = current_map->m_properties;

      if (auto iterator = properties.find(key); iterator != properties.end()) {
        const auto& [dontcare, property] = *iterator;

        if (!property.is_class()) {
          Log::error("Failed to get a property, property is not a class: {}", key);
          throw std::runtime_error("Failed to get a property, property is not a class.");
        }

        current_map = &property.as_class();
      } else {
        Log::error("Failed to get a property, key does not exist: {}", key);
        throw std::runtime_error("Failed to get a property, key does not exist.");
      }
    }

    assert(current_map != nullptr);
    const auto& properties = current_map->m_properties;

    if (auto iterator = properties.find(last_key); iterator != properties.end()) {
      return iterator->second;
    }

    Log::error("Failed to get a property, final key does not exist: {}", last_key);
    throw std::runtime_error("Failed to get a property, final key does not exist.");
  }

  // NOLINTNEXTLINE(misc-no-recursion)
  bool operator==(const PropertyMap& lhs, const PropertyMap& rhs)
  {
    return lhs.m_properties == rhs.m_properties; // NOLINT(misc-no-recursion)
  }

}
