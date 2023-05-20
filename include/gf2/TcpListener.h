// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TCP_LISTENER_H
#define GF_TCP_LISTENER_H

#include <string>
#include <tuple>

#include "NetworkApi.h"
#include "Socket.h"
#include "TcpSocket.h"

namespace gf {

  class GF_NETWORK_API TcpListener : public Socket {
  public:
    TcpListener() = default;
    TcpListener(const std::string& service, SocketFamily family = SocketFamily::Unspec);

    TcpSocket accept();
    std::tuple<TcpSocket, SocketAddress> accept_with_address();
  };

} // namespace gf

#endif // GF_TCP_LISTENER_H
