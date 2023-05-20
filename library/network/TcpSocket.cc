// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/TcpSocket.h>
// clang-format on

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
#include <gf2/Packet.h>

#include "bits/SocketPortability.h"

namespace gf {

  TcpSocket::TcpSocket(const std::string& hostname, const std::string& service, SocketFamily family)
  : Socket(details::native_connect(hostname, service, family))
  {
  }

  TcpSocket::TcpSocket(details::SocketHandle handle)
  : Socket(handle)
  {
  }

  TcpSocket::~TcpSocket()
  {
#ifdef _WIN32
    static constexpr int ReadWrite = SD_BOTH;
#else
    static constexpr int ReadWrite = SHUT_RDWR;
#endif
    ::shutdown(handle(), ReadWrite);
  }

  SocketAddress TcpSocket::remote_address() const
  {
    SocketAddress address;
    auto err = ::getpeername(handle(), address.storage_ptr(), address.length_ptr());

    if (err != 0) {
      Log::error("Could not get the remote address. Reason: {}", details::last_error_string());
    }

    return address;
  }

  SocketDataResult TcpSocket::send_raw_bytes(Span<const uint8_t> buffer)
  {
#ifdef MSG_NOSIGNAL
    static constexpr int SendFlags = MSG_NOSIGNAL;
#else
    static constexpr int SendFlags = details::NoFlag;
#endif
    auto ret = ::send(handle(), details::send_pointer(buffer), details::send_length(buffer), SendFlags);

    if (ret == details::InvalidCommunication) {
      if (details::native_would_block(details::last_error_code())) {
        return { SocketStatus::Block, 0u };
      }

      Log::error("Error while sending data. Reason: {}", details::last_error_string());
      return { SocketStatus::Error, 0u };
    }

    return { SocketStatus::Data, static_cast<std::size_t>(ret) };
  }

  SocketDataResult TcpSocket::recv_raw_bytes(Span<uint8_t> buffer)
  {
    auto ret = ::recv(handle(), details::recv_pointer(buffer), details::recv_length(buffer), details::NoFlag);

    if (ret == details::InvalidCommunication) {
      if (details::native_would_block(details::last_error_code())) {
        return { SocketStatus::Block, 0u };
      }

      Log::error("Error while receiving data. Reason: {}", details::last_error_string());
      return { SocketStatus::Error, 0u };
    }

    if (ret == 0) {
      return { SocketStatus::Close, 0u };
    }

    return { SocketStatus::Data, static_cast<std::size_t>(ret) };
  }

  SocketStatus TcpSocket::send_bytes(Span<const uint8_t> buffer)
  {
    while (!buffer.empty()) {
      auto result = send_raw_bytes(buffer);

      switch (result.status) {
        case SocketStatus::Data:
          buffer = buffer.last_except(result.length);
          break;
        case SocketStatus::Block:
          continue;
        case SocketStatus::Close:
        case SocketStatus::Error:
          return result.status;
      }
    }

    return SocketStatus::Data;
  }

  SocketStatus TcpSocket::recv_bytes(Span<uint8_t> buffer)
  {
    while (!buffer.empty()) {
      auto result = recv_raw_bytes(buffer);

      switch (result.status) {
        case SocketStatus::Data:
          buffer = buffer.last_except(result.length);
          break;
        case SocketStatus::Block:
          continue;
        case SocketStatus::Close:
        case SocketStatus::Error:
          return result.status;
      }
    }

    return SocketStatus::Data;
  }

  SocketStatus TcpSocket::send_packet(const Packet& packet)
  {
    auto size = static_cast<uint64_t>(packet.size());
    auto header = details::encode_header(size);

    auto status = send_bytes(header);

    if (status != SocketStatus::Data) {
      return status;
    }

    return send_bytes(packet.as_span());
  }

  SocketStatus TcpSocket::recv_packet(Packet& packet)
  {
    details::SizeHeader header;
    auto status = recv_bytes(header);

    if (status != SocketStatus::Data) {
      return status;
    }

    auto size = details::decode_header(header);
    packet.ensure_size(static_cast<std::size_t>(size));
    return recv_bytes(packet.as_span());
  }

} // namespace gf
