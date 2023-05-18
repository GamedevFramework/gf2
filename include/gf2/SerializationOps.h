// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SERIALIZATION_OPS_H
#define GF_SERIALIZATION_OPS_H

#include <cstdint>
#include <cstring>

#include <array>
#include <map>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Array2D.h"
#include "CoreApi.h"
#include "Flags.h"
#include "Serialization.h"
#include "Span.h"

namespace gf {

  GF_CORE_API Serializer& operator|(Serializer& ar, bool data);
  GF_CORE_API Serializer& operator|(Serializer& ar, char data);
  GF_CORE_API Serializer& operator|(Serializer& ar, uint8_t data);
  GF_CORE_API Serializer& operator|(Serializer& ar, uint16_t data);
  GF_CORE_API Serializer& operator|(Serializer& ar, uint32_t data);
  GF_CORE_API Serializer& operator|(Serializer& ar, uint64_t data);
  GF_CORE_API Serializer& operator|(Serializer& ar, int8_t data);
  GF_CORE_API Serializer& operator|(Serializer& ar, int16_t data);
  GF_CORE_API Serializer& operator|(Serializer& ar, int32_t data);
  GF_CORE_API Serializer& operator|(Serializer& ar, int64_t data);
  GF_CORE_API Serializer& operator|(Serializer& ar, float data);
  GF_CORE_API Serializer& operator|(Serializer& ar, double data);

#ifndef _MSC_VER
  Serializer& operator|(Serializer& ar, const char* str) = delete;
#endif

  GF_CORE_API Serializer& operator|(Serializer& ar, const std::string& str);

  template<typename T, typename E = std::enable_if_t<std::is_enum_v<T>, T>>
  inline Serializer& operator|(Serializer& ar, T data)
  {
    using U = std::underlying_type_t<T>;
    return ar | static_cast<U>(data);
  }

  template<typename E>
  inline Serializer& operator|(Serializer& ar, Flags<E> data)
  {
    return ar | data.m_data;
  }

  template<typename T>
  Serializer& operator|(Serializer& ar, const Array2D<T>& array)
  {
    const Vec2<int32_t> size = array.size();
    ar | size.w | size.h;

    for (auto& item : array) {
      ar | const_cast<T&>(item); // NOLINT
    }

    return ar;
  }

  template<typename T, std::size_t N>
  inline Serializer& operator|(Serializer& ar, const T (&array)[N])
  {
    ar.write_raw_size(N);

    for (auto& item : array) {
      ar | const_cast<T&>(item); // NOLINT
    }

    return ar;
  }

  template<typename T>
  inline Serializer& operator|(Serializer& ar, const std::vector<T>& array)
  {
    ar.write_raw_size(array.size());

    for (auto& item : array) {
      ar | const_cast<T&>(item); // NOLINT
    }

    return ar;
  }

  template<typename T, std::size_t N>
  inline Serializer& operator|(Serializer& ar, const std::array<T, N>& array)
  {
    ar.write_raw_size(array.size());

    for (auto& item : array) {
      ar | const_cast<T&>(item); // NOLINT
    }

    return ar;
  }

  template<typename T>
  inline Serializer& operator|(Serializer& ar, const std::set<T>& set)
  {
    ar.write_raw_size(set.size());

    for (auto& item : set) {
      ar | const_cast<T&>(item); // NOLINT
    }

    return ar;
  }

  template<typename T>
  inline Serializer& operator|(Serializer& ar, const std::unordered_set<T>& set)
  {
    ar.write_raw_size(set.size());

    for (auto& item : set) {
      ar | const_cast<T&>(item); // NOLINT
    }

    return ar;
  }

  template<typename K, typename V>
  inline Serializer& operator|(Serializer& ar, const std::map<K, V>& map)
  {
    ar.write_raw_size(map.size());

    for (auto& item : map) {
      ar | const_cast<K&>(item.first) | const_cast<V&>(item.second); // NOLINT
    }

    return ar;
  }

  template<typename K, typename V>
  inline Serializer& operator|(Serializer& ar, const std::unordered_map<K, V>& map)
  {
    ar.write_raw_size(map.size());

    for (auto& item : map) {
      ar | const_cast<K&>(item.first) | const_cast<V&>(item.second); // NOLINT
    }

    return ar;
  }

  GF_CORE_API Deserializer& operator|(Deserializer& ar, bool& data);
  GF_CORE_API Deserializer& operator|(Deserializer& ar, char& data);
  GF_CORE_API Deserializer& operator|(Deserializer& ar, uint8_t& data);
  GF_CORE_API Deserializer& operator|(Deserializer& ar, uint16_t& data);
  GF_CORE_API Deserializer& operator|(Deserializer& ar, uint32_t& data);
  GF_CORE_API Deserializer& operator|(Deserializer& ar, uint64_t& data);
  GF_CORE_API Deserializer& operator|(Deserializer& ar, int8_t& data);
  GF_CORE_API Deserializer& operator|(Deserializer& ar, int16_t& data);
  GF_CORE_API Deserializer& operator|(Deserializer& ar, int32_t& data);
  GF_CORE_API Deserializer& operator|(Deserializer& ar, int64_t& data);
  GF_CORE_API Deserializer& operator|(Deserializer& ar, float& data);
  GF_CORE_API Deserializer& operator|(Deserializer& ar, double& data);
  GF_CORE_API Deserializer& operator|(Deserializer& ar, std::string& str);

  template<typename T, typename E = std::enable_if_t<std::is_enum_v<T>, T>>
  inline Deserializer& operator|(Deserializer& ar, T& data)
  {
    using U = std::underlying_type_t<T>;
    U underlying;
    ar | underlying;
    data = static_cast<T>(underlying);
    return ar;
  }

  template<typename E>
  inline Deserializer& operator|(Deserializer& ar, Flags<E>& data)
  {
    return ar | data.m_data;
  }

  template<typename T>
  Deserializer& operator|(Deserializer& ar, Array2D<T>& array)
  {
    Vec2<int32_t> size(0, 0);
    ar | size.w | size.h;

    Array2D<T> tmp(size);

    for (auto& item : tmp) {
      ar | item;
    }

    array = std::move(tmp);
    return ar;
  }

  template<typename T>
  inline Deserializer& operator|(Deserializer& ar, Span<T> array)
  {
    std::size_t size = 0;

    if (!ar.read_raw_size(size)) {
      return ar;
    }

    if (size != array.size()) {
      return ar;
    }

    for (auto& item : array) {
      ar | item;
    }

    return ar;
  }

  template<typename T, std::size_t N>
  inline Deserializer& operator|(Deserializer& ar, T (&array)[N])
  {
    return ar | Span<T>(array);
  }

  template<typename T, std::size_t N>
  inline Deserializer& operator|(Deserializer& ar, std::array<T, N>& array)
  {
    return ar | Span<T>(array);
  }

  template<typename T>
  inline Deserializer& operator|(Deserializer& ar, std::vector<T>& array)
  {
    std::size_t size = 0;

    if (!ar.read_raw_size(size)) {
      return ar;
    }

    array.clear();

    for (std::size_t i = 0; i < size; ++i) {
      T item;
      ar | item;
      array.emplace_back(std::move(item));
    }

    return ar;
  }

  template<typename T>
  inline Deserializer& operator|(Deserializer& ar, std::set<T>& set)
  {
    std::size_t size = 0;

    if (!ar.read_raw_size(size)) {
      return ar;
    }

    set.clear();

    for (std::size_t i = 0; i < size; ++i) {
      T item;
      ar | item;
      set.emplace(std::move(item));
    }

    return ar;
  }

  template<typename T>
  inline Deserializer& operator|(Deserializer& ar, std::unordered_set<T>& set)
  {
    std::size_t size = 0;

    if (!ar.read_raw_size(size)) {
      return ar;
    }

    set.clear();

    for (std::size_t i = 0; i < size; ++i) {
      T item;
      ar | item;
      set.emplace(std::move(item));
    }

    return ar;
  }

  template<typename K, typename V>
  inline Deserializer& operator|(Deserializer& ar, std::map<K, V>& map)
  {
    std::size_t size = 0;

    if (!ar.read_raw_size(size)) {
      return ar;
    }

    map.clear();

    for (std::size_t i = 0; i < size; ++i) {
      K key;
      ar | key;
      V value;
      ar | value;

      map.emplace(std::move(key), std::move(value));
    }

    return ar;
  }

  template<typename K, typename V>
  inline Deserializer& operator|(Deserializer& ar, std::unordered_map<K, V>& map)
  {
    std::size_t size = 0;

    if (!ar.read_raw_size(size)) {
      return ar;
    }

    map.clear();

    for (std::size_t i = 0; i < size; ++i) {
      K key;
      ar | key;
      V value;
      ar | value;

      map.emplace(std::move(key), std::move(value));
    }

    return ar;
  }

} // namespace gf

#endif // GF_SERIALIZATION_OPS_H
