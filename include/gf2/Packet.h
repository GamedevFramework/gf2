/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#ifndef GF_PACKET_H
#define GF_PACKET_H

#include <cassert>
#include <cstdint>

#include <vector>

#include "Id.h"
#include "NetworkApi.h"
#include "Serialization.h"
#include "SerializationOps.h"
#include "Span.h"
#include "Streams.h"

namespace gf {

  class GF_NETWORK_API Packet {
  public:
    Id type()
    {
      if (m_type != InvalidId) {
        return m_type;
      }

      BufferInputStream stream(gf::ref(m_bytes));
      Deserializer deserializer(gf::ref(stream));
      deserializer | m_type;
      return m_type;
    }

    template<typename T>
    T as()
    {
      BufferInputStream stream(gf::ref(m_bytes));
      Deserializer deserializer(gf::ref(stream));

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
      gf::BufferOutputStream stream(gf::ref(m_bytes));
      gf::Serializer serializer(gf::ref(stream));
      serializer | m_type | const_cast<T&>(data); // NOLINT
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
