// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SERIALIZATION_H
#define GF_SERIALIZATION_H

#include <cstddef>
#include <cstdint>

#include "CoreApi.h"
#include "Stream.h"

namespace gf {

  class GF_CORE_API Serializer {
  public:
    template<typename T>
    using Constness = const T;

    Serializer(OutputStream* stream, uint16_t version = 0);

    uint16_t version() const
    {
      return m_version;
    }

    void write_bool(bool data);
    void write_char(char data);
    void write_i8(int8_t data);
    void write_i16(int16_t data);
    void write_i32(int32_t data);
    void write_i64(int64_t data);
    void write_u8(uint8_t data);
    void write_u16(uint16_t data);
    void write_u32(uint32_t data);
    void write_u64(uint64_t data);
    void write_f32(float data);
    void write_f64(double data);
    void write_raw_string(const char* data, std::size_t size);
    void write_raw_size(std::size_t size);

  private:
    void write_big_endian_64(uint64_t data);
    void write_big_endian_32(uint32_t data);
    void write_big_endian_16(uint16_t data);
    void write_big_endian_8(uint8_t data);

    OutputStream* m_stream;
    uint16_t m_version = 0;
  };

  class GF_CORE_API Deserializer {
  public:
    template<typename T>
    using Constness = T;

    Deserializer(InputStream* stream);

    uint16_t version() const
    {
      return m_version;
    }

    void read_bool(bool* data);
    void read_char(char* data);
    void read_i8(int8_t* data);
    void read_i16(int16_t* data);
    void read_i32(int32_t* data);
    void read_i64(int64_t* data);
    void read_u8(uint8_t* data);
    void read_u16(uint16_t* data);
    void read_u32(uint32_t* data);
    void read_u64(uint64_t* data);
    void read_f32(float* data);
    void read_f64(double* data);
    void read_raw_string(char* data, std::size_t size);
    void read_raw_size(std::size_t* size);

  private:
    void read_big_endian_64(uint64_t* data);
    void read_big_endian_32(uint32_t* data);
    void read_big_endian_16(uint16_t* data);
    void read_big_endian_8(uint8_t* data);

    InputStream* m_stream;
    uint16_t m_version = 0;
  };

} // namespace gf

#endif // GF_SERIALIZATION_H
