// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/network/Socket.h>
// clang-format on

#include <utility>

#include <gf2/core/Log.h>

#include "bits/SocketPortability.h"

namespace gf {

  Socket::~Socket()
  {
    if (m_handle != details::InvalidSocketHandle) {
      details::native_close_socket(m_handle);
    }
  }

  Socket::Socket(Socket&& other) noexcept
  : m_handle(std::exchange(other.m_handle, details::InvalidSocketHandle))
  {
  }

  Socket& Socket::operator=(Socket&& other) noexcept
  {
    std::swap(m_handle, other.m_handle);
    return *this;
  }

  SocketAddress Socket::local_address() const
  {
    SocketAddress address;
    auto err = ::getsockname(m_handle, address.storage_ptr(), address.length_ptr());

    if (err != 0) {
      Log::error("Could not get the local address: {}", details::last_error_string());
    }

    return address;
  }

  void Socket::set_blocking() // NOLINT(readability-make-member-function-const)
  {
    if (!details::native_set_block_mode(m_handle, false)) {
      Log::error("Could not set the socket blocking: {}", details::last_error_string());
    }
  }

  void Socket::set_non_blocking() // NOLINT(readability-make-member-function-const)
  {
    if (!details::native_set_block_mode(m_handle, true)) {
      Log::error("Could not set the socket non-blocking: {}", details::last_error_string());
    }
  }

} // namespace gf
