// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SOCKET_TYPES_H
#define GF_SOCKET_TYPES_H

#include <cstddef>
#include <cstdint>

#ifdef _WIN32
#  include <winsock2.h>
#endif

#include <gf2/core/Result.h>

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

  enum class SocketStatus : uint8_t {
    Block,
    Close,
    Error,
  };

  using SocketResult = Result<std::size_t, SocketStatus>;

  enum class SocketSelectorStatus : uint8_t {
    Event,
    Timeout,
    Error,
  };

} // namespace gf

#endif // GF_SOCKET_TYPES_H
