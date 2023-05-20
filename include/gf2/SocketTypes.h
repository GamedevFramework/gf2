// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SOCKET_TYPES_H
#define GF_SOCKET_TYPES_H

#include <cstddef>

#ifdef _WIN32
#  include <winsock2.h>
#endif

#include "NetworkApi.h"

namespace gf {

  namespace details {
#ifdef _WIN32
    using SocketHandle = SOCKET;
    constexpr SocketHandle InvalidSocketHandle = INVALID_SOCKET;
#else
    using SocketHandle = int;
    constexpr SocketHandle InvalidSocketHandle = -1;
#endif
  } // namespace details

  enum class SocketStatus {
    Data,
    Block,
    Close,
    Error,
  };

  struct GF_NETWORK_API SocketDataResult {
    SocketStatus status;
    std::size_t length;
  };

  enum class SocketSelectorStatus {
    Event,
    Timeout,
    Error,
  };

} // namespace gf

#endif // GF_SOCKET_TYPES_H
