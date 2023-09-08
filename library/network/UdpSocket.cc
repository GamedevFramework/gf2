// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/network/UdpSocket.h>
// clang-format on

#include <cassert>
#include <cstring>

#ifdef _WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
#else
#  include <netdb.h>
#  include <sys/socket.h>
#  include <sys/types.h>
#endif

#include <gf2/core/Log.h>

#include "bits/SocketPortability.h"

namespace gf {

  UdpSocket::UdpSocket(const std::string& service, SocketFamily family)
  : Socket(details::native_bind(service, family))
  {
  }

  UdpSocket::UdpSocket(AnyType /* unused */, SocketFamily family)
  : Socket(details::native_bind("0", family))
  {
  }

  SocketAddress UdpSocket::remote_address(const std::string& hostname, const std::string& service)
  {
    auto addresses = details::remote_address_info(hostname, service, details::SocketType::Udp, local_address().family());

    if (!addresses.empty()) {
      return addresses.front().address;
    }

    return {};
  }

  SocketResult UdpSocket::send_raw_bytes_to(Span<const uint8_t> buffer, const SocketAddress& remote_address)
  {
    auto ret = ::sendto(handle(), details::send_pointer(buffer), details::send_length(buffer), details::NoFlag, remote_address.storage_ptr(), remote_address.length());

    if (ret == details::InvalidCommunication) {
      if (details::native_would_block(details::last_error_code())) {
        return error(SocketStatus::Block);
      }

      Log::error("Error while sending data. Reason: {}", details::last_error_string());
      return error(SocketStatus::Error);
    }

    return static_cast<std::size_t>(ret);
  }

  SocketResult UdpSocket::recv_raw_bytes_from(Span<uint8_t> buffer, SocketAddress* remote_address)
  {
    SocketAddress dummy;
    SocketAddress& address = remote_address != nullptr ? *remote_address : dummy;

    auto ret = ::recvfrom(handle(), details::recv_pointer(buffer), details::recv_length(buffer), details::NoFlag, address.storage_ptr(), address.length_ptr());

    if (ret == details::InvalidCommunication) {
      if (details::native_would_block(details::last_error_code())) {
        return error(SocketStatus::Block);
      }

      Log::error("Error while receiving data. Reason: {}", details::last_error_string());
      return error(SocketStatus::Error);
    }

    return static_cast<std::size_t>(ret);
  }

  static constexpr std::size_t MaxDatagramSize = 65507;

  SocketResult UdpSocket::send_bytes_to(Span<const uint8_t> buffer, const SocketAddress& remote_address)
  {
    if (buffer.size() > MaxDatagramSize) {
      return std::size_t(0);
    }

    return send_raw_bytes_to(buffer, remote_address);
  }

  SocketResult UdpSocket::recv_bytes_from(Span<uint8_t> buffer, SocketAddress* remote_address)
  {
    if (buffer.size() > MaxDatagramSize) {
      return std::size_t(0);
    }

    return recv_raw_bytes_from(buffer, remote_address);
  }

} // namespace gf
