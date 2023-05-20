// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TCP_SOCKET_H
#define GF_TCP_SOCKET_H

#include <cstdint>

#include <string>

#include "NetworkApi.h"
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

    SocketDataResult send_raw_bytes(Span<const uint8_t> buffer);
    SocketDataResult recv_raw_bytes(Span<uint8_t> buffer);

    SocketStatus send_bytes(Span<const uint8_t> buffer);
    SocketStatus recv_bytes(Span<uint8_t> buffer);

    SocketStatus send_packet(const Packet& packet);
    SocketStatus recv_packet(Packet& packet);

  private:
    friend class TcpListener;
    TcpSocket(details::SocketHandle handle);
  };
} // namespace gf

#endif // GF_TCP_SOCKET_H
