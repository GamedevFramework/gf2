// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/SerializationOps.h>
// clang-format on

namespace gf {

  /*
   * Serializer
   */

  Serializer& operator|(Serializer& ar, bool data)
  {
    ar.write_bool(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, char data)
  {
    ar.write_char(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, uint8_t data)
  {
    ar.write_u8(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, uint16_t data)
  {
    ar.write_u16(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, uint32_t data)
  {
    ar.write_u32(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, uint64_t data)
  {
    ar.write_u64(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, int8_t data)
  {
    ar.write_i8(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, int16_t data)
  {
    ar.write_i16(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, int32_t data)
  {
    ar.write_i32(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, int64_t data)
  {
    ar.write_i64(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, float data)
  {
    ar.write_f32(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, double data)
  {
    ar.write_f64(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, const std::string& str)
  {
    ar.write_raw_string(str.data(), str.length());
    return ar;
  }

  /*
   * Deserializer
   */

  Deserializer& operator|(Deserializer& ar, bool& data)
  {
    ar.read_bool(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, char& data)
  {
    ar.read_char(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, uint8_t& data)
  {
    ar.read_u8(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, uint16_t& data)
  {
    ar.read_u16(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, uint32_t& data)
  {
    ar.read_u32(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, uint64_t& data)
  {
    ar.read_u64(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, int8_t& data)
  {
    ar.read_i8(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, int16_t& data)
  {
    ar.read_i16(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, int32_t& data)
  {
    ar.read_i32(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, int64_t& data)
  {
    ar.read_i64(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, float& data)
  {
    ar.read_f32(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, double& data)
  {
    ar.read_f64(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, std::string& str)
  {
    std::size_t size = 0;

    if (!ar.read_raw_size(size)) {
      return ar;
    }

    str.resize(size);

    if (!ar.read_raw_string(str.data(), str.size())) {
      return ar;
    }

    return ar;
  }

} // namespace gf
