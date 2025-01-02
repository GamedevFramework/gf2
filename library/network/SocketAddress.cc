// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/network/SocketAddress.h>

#include <cstring>

#include <array>

#ifdef _WIN32
#  include <ws2tcpip.h>
#else
#  include <netdb.h>
#  include <sys/socket.h>
#endif

#include <gf2/core/Log.h>

namespace gf {

  SocketAddress::SocketAddress()
  : m_storage()
  , m_length(sizeof(m_storage))
  {
  }

  SocketAddress::SocketAddress(sockaddr* storage, StorageLengthType length)
  : m_storage()
  , m_length(length)
  {
    std::memcpy(&m_storage, storage, static_cast<std::size_t>(length));
  }

  SocketFamily SocketAddress::family() const
  {
    return static_cast<SocketFamily>(m_storage.ss_family);
  }

  std::string SocketAddress::hostname(SocketAddressFormat format) const
  {
    std::array<char, NI_MAXHOST> host = {};
    auto err = ::getnameinfo(storage_ptr(), length(), host.data(), NI_MAXHOST, nullptr, 0, format == SocketAddressFormat::Numeric ? NI_NUMERICHOST : 0);

    if (err != 0) {
      gf::Log::error("Error while getting the host of an address. Reason: {}", ::gai_strerror(err));
      return "";
    }

    return host.data();
  }

  std::string SocketAddress::service(SocketAddressFormat format) const
  {
    std::array<char, NI_MAXSERV> serv = {};
    auto err = ::getnameinfo(storage_ptr(), length(), nullptr, 0, serv.data(), NI_MAXSERV, format == SocketAddressFormat::Numeric ? NI_NUMERICSERV : 0);

    if (err != 0) {
      gf::Log::error("Error while getting the service of an address. Reason: {}", ::gai_strerror(err));
      return "";
    }

    return serv.data();
  }

} // namespace gf
