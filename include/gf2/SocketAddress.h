// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SOCKET_ADDRESS_H
#define GF_SOCKET_ADDRESS_H

#include <cstdint>

#include <string>

#ifdef _WIN32
#  include <winsock2.h>
#else
#  include <sys/socket.h>
#endif

#include "NetworkApi.h"

namespace gf {

  enum class SocketFamily : int32_t { // NOLINT(performance-enum-size)
    Unspec = AF_UNSPEC,
    IPv4 = AF_INET,
    IPv6 = AF_INET6,
  };

  enum class SocketAddressFormat : uint8_t {
    Unrestricted,
    Numeric,
  };

  class GF_NETWORK_API SocketAddress {
  public:
    SocketAddress();

    SocketFamily family() const;
    std::string hostname(SocketAddressFormat format = SocketAddressFormat::Unrestricted) const;
    std::string service(SocketAddressFormat format = SocketAddressFormat::Unrestricted) const;

    // internal API

#ifdef _WIN32
    using StorageLengthType = int;
#else
    using StorageLengthType = socklen_t;
#endif

    SocketAddress(sockaddr* storage, StorageLengthType length);

    const sockaddr* storage_ptr() const
    {
      return reinterpret_cast<const sockaddr*>(&m_storage); // NOLINT
    }

    sockaddr* storage_ptr()
    {
      return reinterpret_cast<sockaddr*>(&m_storage); // NOLINT
    }

    StorageLengthType length() const
    {
      return m_length;
    }

    StorageLengthType* length_ptr()
    {
      return &m_length;
    }

  private:
    sockaddr_storage m_storage;
    StorageLengthType m_length = 0;
  };

} // namespace gf

#endif // GF_SOCKET_ADDRESS_H
