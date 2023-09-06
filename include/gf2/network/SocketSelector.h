// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SOCKET_SELECTOR_H
#define GF_SOCKET_SELECTOR_H

#include <vector>

#ifdef _WIN32
#  include <winsock2.h>
#else
#  include <poll.h>
#endif

#include <gf2/core/Time.h>

#include "NetworkApi.h"
#include "Socket.h"
#include "SocketTypes.h"

namespace gf {

  class GF_NETWORK_API SocketSelector {
  public:
    SocketSelector() = default;

    void add_socket(const Socket& socket);
    void remove_socket(const Socket& socket);
    void clear();

    SocketSelectorStatus wait(Time duration = seconds(-1));

    bool ready(const Socket& socket);

  private:
    std::vector<pollfd> m_fds;
    bool m_sorted = true;

    decltype(m_fds)::iterator find_socket(const Socket& socket);
  };

} // namespace gf

#endif // GF_SOCKET_SELECTOR_H
