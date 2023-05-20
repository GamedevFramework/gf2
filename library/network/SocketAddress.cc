// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/SocketAddress.h>
// clang-format on

#include <cstring>

#include <array>

#ifdef _WIN32
#  include <ws2tcpip.h>
#else
#  include <sys/socket.h>

#  include <netdb.h>
#endif

#include <gf2/Log.h>

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
    auto err = ::getnameinfo(storage_ptr(), length(), host.data(), host.size(), nullptr, 0, format == SocketAddressFormat::Numeric ? NI_NUMERICHOST : 0);

    if (err != 0) {
      gf::Log::error("Error while getting the host of an address. Reason: {}", ::gai_strerror(err));
      return "";
    }

    return host.data();
  }

  std::string SocketAddress::service(SocketAddressFormat format) const
  {
    std::array<char, NI_MAXSERV> serv = {};
    auto err = ::getnameinfo(storage_ptr(), length(), nullptr, 0, serv.data(), serv.size(), format == SocketAddressFormat::Numeric ? NI_NUMERICSERV : 0);

    if (err != 0) {
      gf::Log::error("Error while getting the service of an address. Reason: {}", ::gai_strerror(err));
      return "";
    }

    return serv.data();
  }

} // namespace gf
