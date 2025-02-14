// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/SecureHash.h>

#include <cassert>

namespace gf {

  namespace {
    constexpr uint32_t ch(uint32_t x, uint32_t y, uint32_t z)
    {
      return (x & (y ^ z)) ^ z; // ==  (x & y) ^ ((~x) & z)
    }

    constexpr uint32_t maj(uint32_t x, uint32_t y, uint32_t z)
    {
      return (x & (y | z)) | (y & z); // == (x & y) ^ (x & z) ^ (y & z)
    }

    constexpr uint32_t shr(uint32_t word, uint32_t bits)
    {
      return word >> bits;
    }

    constexpr uint32_t rotr(uint32_t word, uint32_t bits)
    {
      return (word >> bits) | (word << (32 - bits));
    }

    constexpr uint32_t big_sigma0(uint32_t word)
    {
      return rotr(word, 2) ^ rotr(word, 13) ^ rotr(word, 22);
    }

    constexpr uint32_t big_sigma1(uint32_t word)
    {
      return rotr(word, 6) ^ rotr(word, 11) ^ rotr(word, 25);
    }

    constexpr uint32_t small_sigma0(uint32_t word)
    {
      return rotr(word, 7) ^ rotr(word, 18) ^ shr(word, 3);
    }

    constexpr uint32_t small_sigma1(uint32_t word)
    {
      return rotr(word, 17) ^ rotr(word, 19) ^ shr(word, 10);
    }

  }

  SecureHash::SecureHash()
  {
    reset();
  }

  void SecureHash::reset()
  {
    constexpr std::array<uint32_t, HashSize / 4> H0 = {
      0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
      0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19,
    };

    m_length = 0;
    m_message_block_index = 0;
    m_digest = H0;
  }

  void SecureHash::input(gf::Span<const uint8_t> data)
  {
    for (auto byte : data) {
      m_message_block[m_message_block_index++] = byte; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
      m_length += 8;

      if (m_message_block_index == MessageBlockSize) {
        process_message_block();
      }
    }
  }


  auto SecureHash::result() -> std::array<uint8_t, HashSize>
  {
    finalize(0x80);

    std::array<uint8_t, HashSize> digest = {};

    for (std::size_t i = 0; i < HashSize; ++i) {
      digest[i] = uint8_t(m_digest[i >> 2] >> (8 * (3 - (i & 0x03)))); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    return digest;
  }

  void SecureHash::process_message_block()
  {
    static constexpr std::array<uint32_t, 64> K = {
      0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
      0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
      0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
      0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
      0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
      0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
      0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
      0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
    };

    std::array<uint32_t, 64> w = {};

    for (int i = 0; i < 16; ++i) {
      // clang-format off
      w[i] = (uint32_t(m_message_block[(4 * i) + 0]) << 24) // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
           | (uint32_t(m_message_block[(4 * i) + 1]) << 16) // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
           | (uint32_t(m_message_block[(4 * i) + 2]) << 8)  // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
           | (uint32_t(m_message_block[(4 * i) + 3]) << 0); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
      // clang-format on
    }

    for (int i = 16; i < 64; ++i) {
      w[i] = small_sigma1(w[i - 2]) + w[i - 7] + small_sigma0(w[i - 15]) + w[i - 16]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    uint32_t a = m_digest[0];
    uint32_t b = m_digest[1];
    uint32_t c = m_digest[2];
    uint32_t d = m_digest[3];
    uint32_t e = m_digest[4];
    uint32_t f = m_digest[5];
    uint32_t g = m_digest[6];
    uint32_t h = m_digest[7];

    for (int i = 0; i < 64; ++i) {
      const uint32_t t1 = h + big_sigma1(e) + ch(e, f, g) + K[i] + w[i]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
      const uint32_t t2 = big_sigma0(a) + maj(a, b, c);
      h = g;
      g = f;
      f = e;
      e = d + t1;
      d = c;
      c = b;
      b = a;
      a = t1 + t2;
    }

    m_digest[0] += a;
    m_digest[1] += b;
    m_digest[2] += c;
    m_digest[3] += d;
    m_digest[4] += e;
    m_digest[5] += f;
    m_digest[6] += g;
    m_digest[7] += h;

    m_message_block_index = 0;
  }

  void SecureHash::finalize(uint8_t pad)
  {
    pad_message(pad);

    // zero

    for (auto& byte : m_message_block) {
      byte = 0x00;
    }

    m_length = 0;
  }

  void SecureHash::pad_message(uint8_t pad)
  {
    if (m_message_block_index >= MessageBlockSize - 8) {
      m_message_block[m_message_block_index++] = pad; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

      while (m_message_block_index < MessageBlockSize) {
        m_message_block[m_message_block_index++] = 0x00; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
      }

      process_message_block();
    } else {
      m_message_block[m_message_block_index++] = pad; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    while (m_message_block_index < MessageBlockSize - 8) {
      m_message_block[m_message_block_index++] = 0x00; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    m_message_block[56] = uint8_t(m_length >> 56);
    m_message_block[57] = uint8_t(m_length >> 48);
    m_message_block[58] = uint8_t(m_length >> 40);
    m_message_block[59] = uint8_t(m_length >> 32);
    m_message_block[60] = uint8_t(m_length >> 24);
    m_message_block[61] = uint8_t(m_length >> 16);
    m_message_block[62] = uint8_t(m_length >> 8);
    m_message_block[63] = uint8_t(m_length >> 0);

    process_message_block();
  }


}
