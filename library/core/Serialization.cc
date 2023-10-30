// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/core/Serialization.h>
// clang-format on

#include <cassert>
#include <cstring>

#include <limits>

#include <gf2/core/Log.h>

namespace gf {

  namespace {
    constexpr uint8_t Magic[2] = { 'g', 'f' };

    template<typename To, typename From>
    To bit_cast(From from)
    {
      static_assert(std::is_arithmetic_v<From>, "From should be arithmetic.");
      static_assert(std::is_arithmetic_v<To>, "To should be arithmetic.");
      static_assert(sizeof(From) == sizeof(To), "From and To should be the same size.");
      To to;
      std::memcpy(&to, &from, sizeof(To));
      return to;
    }

  } // namespace

  /*
   * Serializer
   */

  Serializer::Serializer(OutputStream* stream, uint16_t version)
  : m_stream(stream)
  , m_version(version)
  {
    assert(m_stream != nullptr);
    m_stream->write(Magic);
    write_big_endian_16(version);
  }

  void Serializer::write_bool(bool data)
  {
    write_u8(data ? 1 : 0);
  }

  void Serializer::write_char(char data)
  {
    if constexpr (std::numeric_limits<char>::is_signed) {
      write_i8(data);
    } else {
      write_u8(data);
    }
  }

  void Serializer::write_i8(int8_t data)
  {
    write_big_endian_8(bit_cast<uint8_t>(data));
  }

  void Serializer::write_i16(int16_t data)
  {
    write_big_endian_16(bit_cast<uint16_t>(data));
  }

  void Serializer::write_i32(int32_t data)
  {
    write_big_endian_32(bit_cast<uint32_t>(data));
  }

  void Serializer::write_i64(int64_t data)
  {
    write_big_endian_64(bit_cast<uint64_t>(data));
  }

  void Serializer::write_u8(uint8_t data)
  {
    write_big_endian_8(data);
  }

  void Serializer::write_u16(uint16_t data)
  {
    write_big_endian_16(data);
  }

  void Serializer::write_u32(uint32_t data)
  {
    write_big_endian_32(data);
  }

  void Serializer::write_u64(uint64_t data)
  {
    write_big_endian_64(data);
  }

  void Serializer::write_f32(float data)
  {
    static_assert(sizeof(uint32_t) == sizeof(float), "float should be 32 bits");
    write_big_endian_32(bit_cast<uint32_t>(data));
  }

  void Serializer::write_f64(double data)
  {
    static_assert(sizeof(uint64_t) == sizeof(double), "double should be 64 bits");
    write_big_endian_64(bit_cast<uint64_t>(data));
  }

  void Serializer::write_raw_string(const char* data, std::size_t size)
  {
    write_raw_size(size);

    if (size > 0) {
      m_stream->write(gf::span(reinterpret_cast<const uint8_t*>(data), size)); // NOLINT
    }
  }

  namespace {

    template<typename I, std::size_t N>
    void write_integer(I data, uint8_t (&buf)[N])
    {
      for (std::size_t i = 0; i < N; ++i) {
        buf[N - i - 1] = static_cast<uint8_t>(data >> (8 * i));
      }
    }

    template<std::size_t Size>
    void write_transformed_integer(OutputStream* stream, uint64_t size)
    {
      constexpr std::size_t Offset = (UINT64_C(1) << ((Size - 1) * 8)) - 1;
      size -= Offset;
      assert(size < (Offset << 8));
      uint8_t buf[Size];
      write_integer(size, buf);
      assert(buf[0] != 0xFF || size == UINT64_C(0xFF00000000000000));
      stream->write(buf);
    }

  } // namespace

  /* As size is used many times in the library (for every container), and
   * as small size are the common case, this function use a variable-length
   * encoding for size.
   *
   * Size between 0 and 0xFE (included) are encoded as is.
   *
   * Size between 0xFF and 0xFFFE (included) are encoded as follows:
   * - first a 0xFF is written in the stream
   * - then an offset of 0xFF is substracted from the size so that it is now
   *   in the range [ 0x00, 0xFEFF ], meaning that no offseted size starts
   *   with 0xFF. The result is written as a big endian 2 byte number
   *
   * Size between 0xFFFF and 0xFFFFFE (included) are encoded as follows:
   * - first two 0xFF are written the stream
   * - then an offset of 0xFFFF is substracted from the size so that is is now
   *   in the range [ 0x00, 0xFEFFFF ], meaning that no offseted size starts
   *   with 0xFF. The result is written as a big endian 3 byte number.
   *
   * And so on.
   *
   * The encoded size of a size is as follows:
   * from...            to...               encoded size
   * 0x00               0xFE                1 byte
   * 0xFF               0xFFFE              3 bytes
   * 0xFFFF             0xFFFFFE            5 bytes
   * 0xFFFFFF           0xFFFFFFFE          7 bytes
   * 0xFFFFFFFF         0xFFFFFFFFFE        9 bytes
   * 0xFFFFFFFFFF       0xFFFFFFFFFFFE      11 bytes
   * 0xFFFFFFFFFFFF     0xFFFFFFFFFFFFFE    13 bytes
   * 0xFFFFFFFFFFFFFF   0xFFFFFFFFFFFFFFFF  15 bytes
   *
   * Which means that for values under 0xFFFFFFFE = 2^32 - 1, the encoded size is strictly less than the encoded size of a 64 bit integer.
   */

  void Serializer::write_raw_size(std::size_t raw_size)
  {
    auto size = static_cast<uint64_t>(raw_size);

    if (size < 0xFF) {
      m_stream->write(static_cast<uint8_t>(size));
      return;
    }

    write_u8(0xFF);

    if (size < 0xFFFF) {
      write_transformed_integer<2>(m_stream, size);
      return;
    }

    write_u8(0xFF);

    if (size < 0xFFFFFF) {
      write_transformed_integer<3>(m_stream, size);
      return;
    }

    write_u8(0xFF);

    if (size < 0xFFFFFFFF) {
      write_transformed_integer<4>(m_stream, size);
      return;
    }

    write_u8(0xFF);

    if (size < 0xFFFFFFFFFF) {
      write_transformed_integer<5>(m_stream, size);
      return;
    }

    write_u8(0xFF);

    if (size < 0xFFFFFFFFFFFF) {
      write_transformed_integer<6>(m_stream, size);
      return;
    }

    write_u8(0xFF);

    if (size < 0xFFFFFFFFFFFFFF) {
      write_transformed_integer<7>(m_stream, size);
      return;
    }

    write_u8(0xFF);
    write_transformed_integer<8>(m_stream, size);
  }

  void Serializer::write_big_endian_64(uint64_t data)
  {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];
    write_integer(data, buf);
    m_stream->write(buf);
  }

  void Serializer::write_big_endian_32(uint32_t data)
  {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];
    write_integer(data, buf);
    m_stream->write(buf);
  }

  void Serializer::write_big_endian_16(uint16_t data)
  {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];
    write_integer(data, buf);
    m_stream->write(buf);
  }

  void Serializer::write_big_endian_8(uint8_t data)
  {
    m_stream->write(data);
  }

  /*
   * Deserializer
   */

  Deserializer::Deserializer(InputStream* stream)
  : m_stream(stream)
  {
    assert(m_stream != nullptr);
    uint8_t magic[2] = { 0u, 0u };
    m_stream->read(magic);

    if (magic[0] != Magic[0] || magic[1] != Magic[1]) {
      Log::fatal("The stream is not a gf archive.");
    }

    read_big_endian_16(&m_version);
  }

  void Deserializer::read_bool(bool* data)
  {
    assert(data != nullptr);
    uint8_t x = 0;
    read_big_endian_8(&x);
    *data = (x != 0);
  }

  void Deserializer::read_char(char* data)
  {
    assert(data != nullptr);

    if constexpr (std::numeric_limits<char>::is_signed) {
      int8_t x = 0;
      read_i8(&x);
      *data = static_cast<char>(x);
    } else {
      uint8_t x = 0;
      read_u8(&x);
      *data = static_cast<char>(x);
    }
  }

  void Deserializer::read_i8(int8_t* data)
  {
    assert(data != nullptr);
    uint8_t x = 0;
    read_big_endian_8(&x);
    *data = bit_cast<int8_t>(x);
  }

  void Deserializer::read_i16(int16_t* data)
  {
    assert(data != nullptr);
    uint16_t x = 0;
    read_big_endian_16(&x);
    *data = bit_cast<int16_t>(x);
  }

  void Deserializer::read_i32(int32_t* data)
  {
    assert(data != nullptr);
    uint32_t x = 0;
    read_big_endian_32(&x);
    *data = bit_cast<int32_t>(x);
  }

  void Deserializer::read_i64(int64_t* data)
  {
    assert(data != nullptr);
    uint64_t x = 0;
    read_big_endian_64(&x);
    *data = bit_cast<int64_t>(x);
  }

  void Deserializer::read_u8(uint8_t* data)
  {
    assert(data != nullptr);
    read_big_endian_8(data);
  }

  void Deserializer::read_u16(uint16_t* data)
  {
    assert(data != nullptr);
    read_big_endian_16(data);
  }

  void Deserializer::read_u32(uint32_t* data)
  {
    assert(data != nullptr);
    read_big_endian_32(data);
  }

  void Deserializer::read_u64(uint64_t* data)
  {
    assert(data != nullptr);
    read_big_endian_64(data);
  }

  void Deserializer::read_f32(float* data)
  {
    assert(data != nullptr);
    uint32_t x = 0;
    read_big_endian_32(&x);
    *data = bit_cast<float>(x);
  }

  void Deserializer::read_f64(double* data)
  {
    assert(data != nullptr);
    uint64_t x = 0;
    read_big_endian_64(&x);
    *data = bit_cast<double>(x);
  }

  void Deserializer::read_raw_string(char* data, std::size_t size)
  {
    assert(data != nullptr);

    if (m_stream->read(gf::span(reinterpret_cast<uint8_t*>(data), size)) != size) { // NOLINT
      Log::fatal("End of stream while reading string.");
    }
  }

  namespace {

    template<typename I, std::size_t N>
    I read_integer(uint8_t (&buf)[N])
    {
      static_assert(sizeof(I) <= N, "Size mismatch.");
      I result = 0;

      for (std::size_t i = 0; i < N; ++i) {
        result = (result << 8) + buf[i];
      }

      return result;
    }

  } // namespace

  void Deserializer::read_raw_size(std::size_t* raw_size)
  {
    assert(raw_size != nullptr);
    static constexpr std::size_t Size = sizeof(uint64_t);

    uint8_t data = 0;
    read_u8(&data);

    if (data < 0xFF) {
      // early exit
      *raw_size = data;
      return;
    }

    read_u8(&data);
    std::size_t n = 1;

    while (data == 0xFF && n < Size - 1) {
      read_u8(&data);
      ++n;
    }

    assert(n < Size);
    uint8_t buf[Size];
    const std::size_t m = Size - n - 1;

    for (std::size_t i = 0; i < m; ++i) {
      buf[i] = 0x00; // NOLINT
    }

    buf[m] = data; // NOLINT

    for (std::size_t i = m + 1; i < Size; ++i) {
      read_u8(&buf[i]); // NOLINT
    }

    auto size = read_integer<uint64_t>(buf);

    static constexpr uint64_t Offsets[] = {
      0x00,
      UINT64_C(0xFF),
      UINT64_C(0xFFFF),
      UINT64_C(0xFFFFFF),
      UINT64_C(0xFFFFFFFF),
      UINT64_C(0xFFFFFFFFFF),
      UINT64_C(0xFFFFFFFFFFFF),
      UINT64_C(0xFFFFFFFFFFFFFF),
    };

    assert(n < std::size(Offsets));
    size += Offsets[n]; // NOLINT

    *raw_size = static_cast<std::size_t>(size);
  }

  void Deserializer::read_big_endian_64(uint64_t* data)
  {
    assert(data != nullptr);
    static constexpr std::size_t Size = sizeof(*data);
    uint8_t buf[Size];

    if (m_stream->read(buf) != Size) {
      Log::fatal("End of stream while reading 8 bytes.");
    }

    *data = read_integer<uint64_t>(buf);
  }

  void Deserializer::read_big_endian_32(uint32_t* data)
  {
    assert(data != nullptr);
    static constexpr std::size_t Size = sizeof(*data);
    uint8_t buf[Size];

    if (m_stream->read(buf) != Size) { // Flawfinder: ignore
      Log::fatal("End of stream while reading 4 bytes.");
    }

    *data = read_integer<uint32_t>(buf);
  }

  void Deserializer::read_big_endian_16(uint16_t* data)
  {
    assert(data != nullptr);
    static constexpr std::size_t Size = sizeof(*data);
    uint8_t buf[Size];

    if (m_stream->read(buf) != Size) {
      Log::fatal("End of stream while reading 2 bytes.");
    }

    *data = read_integer<uint16_t>(buf);
  }

  void Deserializer::read_big_endian_8(uint8_t* data)
  {
    assert(data != nullptr);
    if (m_stream->read(*data) != 1) {
      Log::fatal("End of stream while reading 1 byte.");
    }
  }

} // namespace gf
