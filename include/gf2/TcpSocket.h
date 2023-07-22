// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TCP_SOCKET_H
#define GF_TCP_SOCKET_H

#include <cstdint>

#include <string>

#include "NetworkApi.h"
#include "Result.h"
#include "Socket.h"
#include "Span.h"

namespace gf {
  class Packet;

  class GF_NETWORK_API TcpSocket : public Socket {
  public:
    TcpSocket() = default;
    TcpSocket(const std::string& hostname, const std::string& service, SocketFamily family = SocketFamily::Unspec);
    TcpSocket(const TcpSocket&) = delete;
    TcpSocket(TcpSocket&&) = default;
    ~TcpSocket();

    TcpSocket& operator=(const TcpSocket&) = delete;
    TcpSocket& operator=(TcpSocket&&) = default;

    SocketAddress remote_address() const;

    SocketResult send_raw_bytes(Span<const uint8_t> buffer);
    SocketResult recv_raw_bytes(Span<uint8_t> buffer);

    SocketResult send_bytes(Span<const uint8_t> buffer);
    SocketResult recv_bytes(Span<uint8_t> buffer);

    SocketResult send_packet(const Packet& packet);
    SocketResult recv_packet(Packet& packet);

  private:
    friend class TcpListener;
    TcpSocket(details::SocketHandle handle);
  };
} // namespace gf

#endif // GF_TCP_SOCKET_H
