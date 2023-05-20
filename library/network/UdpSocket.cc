// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/UdpSocket.h>
// clang-format on

#include <cassert>
#include <cstring>

#ifdef _WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
#else
#  include <sys/socket.h>
#  include <sys/types.h>

#  include <netdb.h>
#endif

#include <gf2/Log.h>

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

  SocketDataResult UdpSocket::send_raw_bytes_to(Span<const uint8_t> buffer, const SocketAddress& address)
  {
    auto ret = ::sendto(handle(), details::send_pointer(buffer), details::send_length(buffer), details::NoFlag, address.storage_ptr(), address.length());

    if (ret == details::InvalidCommunication) {
      if (details::native_would_block(details::last_error_code())) {
        return { SocketStatus::Block, 0u };
      }

      Log::error("Error while sending data. Reason: {}", details::last_error_string());
      return { SocketStatus::Error, 0u };
    }

    return { SocketStatus::Data, static_cast<std::size_t>(ret) };
  }

  std::tuple<SocketDataResult, SocketAddress> UdpSocket::recv_raw_bytes_from(Span<uint8_t> buffer)
  {
    SocketAddress address;
    auto ret = ::recvfrom(handle(), details::recv_pointer(buffer), details::recv_length(buffer), details::NoFlag, address.storage_ptr(), address.length_ptr());

    if (ret == details::InvalidCommunication) {
      if (details::native_would_block(details::last_error_code())) {
        const SocketDataResult result = { SocketStatus::Block, 0u };
        return std::make_tuple(result, address);
      }

      Log::error("Error while receiving data. Reason: {}", details::last_error_string());
      const SocketDataResult result = { SocketStatus::Error, 0u };
      return std::make_tuple(result, address);
    }

    const SocketDataResult result = { SocketStatus::Data, static_cast<std::size_t>(ret) };
    return std::make_tuple(result, address);
  }

  static constexpr std::size_t MaxDatagramSize = 65507;

  bool UdpSocket::send_bytes_to(Span<const uint8_t> buffer, const SocketAddress& address)
  {
    if (buffer.size() > MaxDatagramSize) {
      return false;
    }

    auto result = send_raw_bytes_to(buffer, address);

    if (result.status == SocketStatus::Data) {
      return result.length == buffer.size();
    }

    assert(result.status != SocketStatus::Close);
    return false;
  }

  std::tuple<bool, SocketAddress> UdpSocket::recv_bytes_from(Span<uint8_t> buffer)
  {
    if (buffer.size() > MaxDatagramSize) {
      return std::make_tuple(false, SocketAddress());
    }

    auto [result, address] = recv_raw_bytes_from(buffer);

    if (result.status == SocketStatus::Data) {
      return std::make_tuple(result.length == buffer.size(), address);
    }

    assert(result.status != SocketStatus::Close);
    return std::make_tuple(false, address);
  }

} // namespace gf
