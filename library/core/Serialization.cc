// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Serialization.h>
// clang-format on

#include <cassert>
#include <cinttypes>
#include <cstring>

#include <limits>

#include <gf2/Log.h>

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
      Log::error("The stream is not a gf archive."); // throw?
      return;
    }

    read_big_endian_16(m_version);
  }

  bool Deserializer::read_bool(bool& data)
  {
    uint8_t x = 0;

    if (!read_big_endian_8(x)) {
      Log::error("Asking for a boolean but the file is at the end.");
      return false;
    }

    data = (x != 0);
    return true;
  }

  bool Deserializer::read_char(char& data)
  {
    if constexpr (std::numeric_limits<char>::is_signed) {
      int8_t x = 0;

      if (!read_i8(x)) {
        Log::error("Asking for a char but the file is at the end.");
        return false;
      }

      data = static_cast<char>(x);
    } else {
      uint8_t x = 0;

      if (!read_u8(x)) {
        Log::error("Asking for a char but the file is at the end.");
        return false;
      }

      data = static_cast<char>(x);
    }

    return true;
  }

  bool Deserializer::read_i8(int8_t& data)
  {
    uint8_t x = 0;

    if (!read_big_endian_8(x)) {
      Log::error("Asking for a signed integer but the file is at the end.");
      return false;
    }

    data = bit_cast<int8_t>(x);
    return true;
  }

  bool Deserializer::read_i16(int16_t& data)
  {
    uint16_t x = 0;

    if (!read_big_endian_16(x)) {
      Log::error("Asking for a signed integer but the file is at the end.");
      return false;
    }

    data = bit_cast<int16_t>(x);
    return true;
  }

  bool Deserializer::read_i32(int32_t& data)
  {
    uint32_t x = 0;

    if (!read_big_endian_32(x)) {
      Log::error("Asking for a signed integer but the file is at the end.");
      return false;
    }

    data = bit_cast<int32_t>(x);
    return true;
  }

  bool Deserializer::read_i64(int64_t& data)
  {
    uint64_t x = 0;

    if (!read_big_endian_64(x)) {
      Log::error("Asking for a signed integer but the file is at the end.");
      return false;
    }

    data = bit_cast<int64_t>(x);
    return true;
  }

  bool Deserializer::read_u8(uint8_t& data)
  {
    if (!read_big_endian_8(data)) {
      Log::error("Asking for an unsigned integer but the file is at the end.");
      return false;
    }

    return true;
  }

  bool Deserializer::read_u16(uint16_t& data)
  {
    if (!read_big_endian_16(data)) {
      Log::error("Asking for an unsigned integer but the file is at the end.");
      return false;
    }

    return true;
  }

  bool Deserializer::read_u32(uint32_t& data)
  {
    if (!read_big_endian_32(data)) {
      Log::error("Asking for an unsigned integer but the file is at the end.");
      return false;
    }

    return true;
  }

  bool Deserializer::read_u64(uint64_t& data)
  {
    if (!read_big_endian_64(data)) {
      Log::error("Asking for an unsigned integer but the file is at the end.");
      return false;
    }

    return true;
  }

  bool Deserializer::read_f32(float& data)
  {
    uint32_t x = 0;

    if (!read_big_endian_32(x)) {
      Log::error("Asking for a float but the file is at the end.");
      return false;
    }

    data = bit_cast<float>(x);
    return true;
  }

  bool Deserializer::read_f64(double& data)
  {
    uint64_t x = 0;

    if (!read_big_endian_64(x)) {
      Log::error("Asking for a double but the file is at the end.");
      return false;
    }

    data = bit_cast<double>(x);
    return true;
  }

  bool Deserializer::read_raw_string(char* data, std::size_t size)
  {
    if (m_stream->read(gf::span(reinterpret_cast<uint8_t*>(data), size)) != size) { // NOLINT
      Log::error("Asking for a string but the file is at the end.");
      return false;
    }

    return true;
  }

  namespace {

    template<typename I, std::size_t N>
    I read_integer(uint8_t (&buf)[N])
    {
      I result = 0;

      for (std::size_t i = 0; i < N; ++i) {
        result = (result << 8) + buf[i];
      }

      return result;
    }

  } // namespace

  bool Deserializer::read_raw_size(std::size_t& raw_size)
  {
    static constexpr std::size_t Size = sizeof(uint64_t);

    uint8_t data = 0;
    std::size_t n = 0;

    for (;;) {
      if (!read_u8(data)) {
        return false;
      }

      if (data != 0xFF || n == Size - 1) {
        break;
      }

      ++n;
    }

    if (n == 0) {
      // early exit
      raw_size = data;
      return true;
    }

    assert(n < Size);
    uint8_t buf[Size];
    const std::size_t m = Size - n - 1;

    for (std::size_t i = 0; i < m; ++i) {
      buf[i] = 0x00; // NOLINT
    }

    buf[m] = data; // NOLINT

    for (std::size_t i = m + 1; i < Size; ++i) {
      if (!read_u8(buf[i])) { // NOLINT
        return false;
      }
    }

    auto size = read_integer<uint64_t>(buf);

    switch (n) {
      case 1:
        size += 0xFF;
        break;
      case 2:
        size += 0xFFFF;
        break;
      case 3:
        size += 0xFFFFFF;
        break;
      case 4:
        size += 0xFFFFFFFF;
        break;
      case 5:
        size += 0xFFFFFFFFFF;
        break;
      case 6:
        size += 0xFFFFFFFFFFFF;
        break;
      case 7:
        size += 0xFFFFFFFFFFFFFF;
        break;
      case 8:
        size += 0xFFFFFFFFFFFFFFFF;
        break;
      default:
        assert(false);
        break;
    }

    raw_size = static_cast<std::size_t>(size);
    return true;
  }

  bool Deserializer::read_big_endian_64(uint64_t& data)
  {
    static constexpr std::size_t Size = sizeof(data);
    uint8_t buf[Size];

    if (m_stream->read(buf) != Size) {
      return false;
    }

    data = read_integer<uint64_t>(buf);
    return true;
  }

  bool Deserializer::read_big_endian_32(uint32_t& data)
  {
    static constexpr std::size_t Size = sizeof(data);
    uint8_t buf[Size];

    if (m_stream->read(buf) != Size) { // Flawfinder: ignore
      return false;
    }

    data = read_integer<uint32_t>(buf);
    return true;
  }

  bool Deserializer::read_big_endian_16(uint16_t& data)
  {
    static constexpr std::size_t Size = sizeof(data);
    uint8_t buf[Size];

    if (m_stream->read(buf) != Size) {
      return false;
    }

    data = read_integer<uint16_t>(buf);
    return true;
  }

  bool Deserializer::read_big_endian_8(uint8_t& data)
  {
    return m_stream->read(data) == 1;
  }

} // namespace gf
