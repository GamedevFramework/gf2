// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PACKET_H
#define GF_PACKET_H

#include <cassert>
#include <cstdint>

#include <vector>

#include <gf2/core/Id.h>
#include <gf2/core/Serialization.h>
#include <gf2/core/SerializationOps.h>
#include <gf2/core/Span.h>
#include <gf2/core/Streams.h>

#include "NetworkApi.h"

namespace gf {

  class GF_NETWORK_API Packet {
  public:
    Id type()
    {
      if (m_type != InvalidId) {
        return m_type;
      }

      BufferInputStream stream(&m_bytes);
      Deserializer deserializer(&stream);
      deserializer | m_type;
      return m_type;
    }

    template<typename T>
    T as()
    {
      BufferInputStream stream(&m_bytes);
      Deserializer deserializer(&stream);

      T data;
      deserializer | m_type | data;
      assert(T::type == m_type);
      return data;
    }

    template<typename T>
    void set(const T& data)
    {
      m_bytes.clear();
      m_type = T::type;
      BufferOutputStream stream(&m_bytes);
      Serializer serializer(&stream);
      serializer | m_type | data;
    }

    std::size_t size() const
    {
      return m_bytes.size();
    }

    void ensure_size(std::size_t size)
    {
      m_bytes.resize(size);
    }

    Span<uint8_t> as_span()
    {
      return { m_bytes };
    }

    Span<const uint8_t> as_span() const
    {
      return { m_bytes };
    }

  private:
    Id m_type = InvalidId;
    std::vector<uint8_t> m_bytes;
  };

} // namespace gf

#endif // GF_PACKET_H
