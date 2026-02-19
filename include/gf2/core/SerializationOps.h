// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SERIALIZATION_OPS_H
#define GF_SERIALIZATION_OPS_H

#include <cstdint>
#include <cstring>

#include <filesystem>
#include <string>
#include <type_traits>

#include "BinaryHeap.h"
#include "Flags.h"
#include "Serialization.h"

namespace gf {

  /*
   * Serializer
   */

  inline Serializer& operator|(Serializer& ar, bool data)
  {
    ar.write_bool(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, char data)
  {
    ar.write_char(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, uint8_t data)
  {
    ar.write_u8(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, uint16_t data)
  {
    ar.write_u16(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, uint32_t data)
  {
    ar.write_u32(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, uint64_t data)
  {
    ar.write_u64(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, int8_t data)
  {
    ar.write_i8(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, int16_t data)
  {
    ar.write_i16(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, int32_t data)
  {
    ar.write_i32(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, int64_t data)
  {
    ar.write_i64(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, float data)
  {
    ar.write_f32(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, double data)
  {
    ar.write_f64(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, char16_t data)
  {
    ar.write_u16(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, char32_t data)
  {
    ar.write_u32(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, const std::string& str)
  {
    ar.write_raw_string(str.data(), str.length());
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, const std::filesystem::path& path)
  {
    return ar | path.generic_string();
  }

  template<typename T, typename E = std::enable_if_t<std::is_enum_v<T>, T>>
  Serializer& operator|(Serializer& ar, T data)
  {
    using U = std::underlying_type_t<T>;
    return ar | static_cast<U>(data);
  }

  template<typename E>
  Serializer& operator|(Serializer& ar, Flags<E> data)
  {
    return ar | data.m_data;
  }

  template<typename T, typename Compare>
  Serializer& operator|(Serializer& ar, const BinaryHeap<T, Compare>& data)
  {
    ar.write_raw_size(data.m_size);

    for (std::size_t i = 0; i < data.m_size; ++i) {
      ar | data.m_elements[i].element;
    }

    return ar;
  }

  template<typename K, typename V>
  Serializer& operator|(Serializer& ar, const std::pair<K, V>& pair)
  {
    return ar | pair.first | pair.second;
  }

  /*
   * Deserializer
   */

  inline Deserializer& operator|(Deserializer& ar, bool& data)
  {
    ar.read_bool(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, char& data)
  {
    ar.read_char(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, uint8_t& data)
  {
    ar.read_u8(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, uint16_t& data)
  {
    ar.read_u16(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, uint32_t& data)
  {
    ar.read_u32(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, uint64_t& data)
  {
    ar.read_u64(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, int8_t& data)
  {
    ar.read_i8(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, int16_t& data)
  {
    ar.read_i16(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, int32_t& data)
  {
    ar.read_i32(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, int64_t& data)
  {
    ar.read_i64(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, float& data)
  {
    ar.read_f32(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, double& data)
  {
    ar.read_f64(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, char16_t& data)
  {
    uint16_t tmp = 0;
    ar.read_u16(&tmp);
    data = tmp;
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, char32_t& data)
  {
    uint32_t tmp = 0;
    ar.read_u32(&tmp);
    data = tmp;
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, std::string& str)
  {
    std::size_t size = 0;
    ar.read_raw_size(&size);
    str.resize(size);
    ar.read_raw_string(str.data(), str.size());
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, std::filesystem::path& path)
  {
    std::string str;
    ar | str;
    path = std::move(str);
    return ar;
  }

  template<typename T, typename E = std::enable_if_t<std::is_enum_v<T>, T>>
  Deserializer& operator|(Deserializer& ar, T& data)
  {
    using U = std::underlying_type_t<T>;
    U underlying;
    ar | underlying;
    data = static_cast<T>(underlying);
    return ar;
  }

  template<typename E>
  Deserializer& operator|(Deserializer& ar, Flags<E>& data)
  {
    return ar | data.m_data;
  }

  template<typename T, typename Compare>
  Deserializer& operator|(Deserializer& ar, BinaryHeap<T, Compare>& data)
  {
    std::size_t size = 0;
    ar.read_raw_size(&size);

    for (std::size_t i = 0; i < size; ++i) {
      T element;
      ar | element;
      data.push(std::move(element));
    }

    return ar;
  }

  template<typename K, typename V>
  Deserializer& operator|(Deserializer& ar, std::pair<K, V>& pair)
  {
    return ar | pair.first | pair.second;
  }

} // namespace gf

#endif // GF_SERIALIZATION_OPS_H
