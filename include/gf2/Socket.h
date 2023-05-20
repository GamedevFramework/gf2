// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SOCKET_H
#define GF_SOCKET_H

#include "NetworkApi.h"
#include "SocketAddress.h"
#include "SocketTypes.h"

namespace gf {

  class GF_NETWORK_API Socket {
  public:
    Socket(Socket&& other) noexcept;
    Socket(const Socket&) = delete;
    ~Socket();

    Socket& operator=(const Socket&) = delete;
    Socket& operator=(Socket&& other) noexcept;

    operator bool() const noexcept
    {
      return m_handle != details::InvalidSocketHandle;
    }

    SocketAddress local_address() const;

    void set_blocking();
    void set_non_blocking();

  protected:
    Socket()
    : m_handle(details::InvalidSocketHandle)
    {
    }

    Socket(details::SocketHandle handle)
    : m_handle(handle)
    {
    }

    details::SocketHandle handle() const
    {
      return m_handle;
    }

  private:
    friend class SocketSelector;
    details::SocketHandle m_handle;
  };

} // namespace gf

#endif // GF_SOCKET_H
