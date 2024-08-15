// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/PropertyMap.h>

#include <cassert>

#include <gf2/core/Log.h>
#include <gf2/core/Property.h>
#include <gf2/core/StringUtils.h>

namespace gf {

  void PropertyMap::add_property(std::string key, bool value)
  {
    m_properties.emplace(std::move(key), Property(value));
  }

  void PropertyMap::add_property(std::string key, int64_t value)
  {
    m_properties.emplace(std::move(key), Property(value));
  }

  void PropertyMap::add_property(std::string key, double value)
  {
    m_properties.emplace(std::move(key), Property(value));
  }

  void PropertyMap::add_property(std::string key, std::string value)
  {
    m_properties.emplace(std::move(key), Property(std::move(value)));
  }

  void PropertyMap::add_property(std::string key, std::filesystem::path value)
  {
    m_properties.emplace(std::move(key), Property(std::move(value)));
  }

  void PropertyMap::add_property(std::string key, Id value)
  {
    m_properties.emplace(std::move(key), Property(value));
  }

  void PropertyMap::add_property(std::string key, Color value)
  {
    m_properties.emplace(std::move(key), Property(value));
  }

  void PropertyMap::add_property(std::string key, PropertyMap value)
  {
    m_properties.emplace(std::move(key), Property(std::move(value)));
  }

  bool PropertyMap::has_property(std::string_view key) const
  {
    return m_properties.find(key) != m_properties.end();
  }

  const Property& PropertyMap::operator()(std::string_view path) const
  {
    auto keys = split_path(path);

    if (keys.empty()) {
      Log::fatal("Failed to get a property, empty path.");
    }

    std::string_view last_key = keys.back();
    keys.pop_back();

    const PropertyMap* current_map = this;

    for (auto key : keys) {
      const auto& properties = current_map->m_properties;

      if (auto iterator = properties.find(key); iterator != properties.end()) {
        const auto& [dontcare, property] = *iterator;

        if (!property.is_class()) {
          Log::fatal("Failed to get a property, property is not a class: {}", key);
        }

        current_map = &property.as_class();
      } else {
        Log::fatal("Failed to get a property, key does not exist: {}", key);
      }
    }

    assert(current_map != nullptr);
    const auto& properties = current_map->m_properties;

    if (auto iterator = properties.find(last_key); iterator != properties.end()) {
      return iterator->second;
    }

    Log::fatal("Failed to get a property, final key does not exist: {}", last_key);
  }

  // NOLINTNEXTLINE(misc-no-recursion)
  bool operator==(const PropertyMap& lhs, const PropertyMap& rhs)
  {
    return lhs.m_properties == rhs.m_properties; // NOLINT(misc-no-recursion)
  }

}
