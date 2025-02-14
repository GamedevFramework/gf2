// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SECURE_HASH_H
#define GF_SECURE_HASH_H

#include <cstdint>

#include <array>

#include "CoreApi.h"
#include "Span.h"

namespace gf {

  class GF_CORE_API SecureHash {
  public:
    static constexpr std::size_t HashSize = 32;
    using Hash = std::array<uint8_t, HashSize>;

    SecureHash();

    void reset();
    void input(Span<const uint8_t> data);
    Hash result();

  private:
    void process_message_block();
    void finalize(uint8_t pad);
    void pad_message(uint8_t pad);

    std::array<uint32_t, HashSize / 4> m_digest = {};
    uint64_t m_length = 0;

    static constexpr std::size_t MessageBlockSize = 64;
    uint32_t m_message_block_index = 0;
    std::array<uint8_t, MessageBlockSize> m_message_block = {};
  };

}

#endif // GF_SECURE_HASH_H
