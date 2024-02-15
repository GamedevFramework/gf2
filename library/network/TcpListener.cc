// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/network/TcpListener.h>

#include <cstring>

#ifdef _WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
#else
#  include <sys/socket.h>
#  include <sys/types.h>
#endif

#include <gf2/core/Log.h>

#include "bits/SocketPortability.h"

namespace gf {

  TcpListener::TcpListener(const std::string& service, SocketFamily family)
  : Socket(details::native_bind_listen(service, family))
  {
  }

  TcpSocket TcpListener::accept()
  {
    const details::SocketHandle socket = ::accept(handle(), nullptr, nullptr);

    if (socket == details::InvalidSocketHandle) {
      Log::error("Error while accepting. Reason: {}", details::last_error_string());
    }

    return { socket };
  }

  std::tuple<TcpSocket, SocketAddress> TcpListener::accept_with_address()
  {
    SocketAddress address;
    const details::SocketHandle socket = ::accept(handle(), address.storage_ptr(), address.length_ptr());

    if (socket == details::InvalidSocketHandle) {
      Log::error("Error while accepting. Reason: {}", details::last_error_string());
    }

    return std::make_tuple(TcpSocket(socket), address);
  }

} // namespace gf
