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

#include "NetworkApi.h"
#include "Socket.h"
#include "SocketTypes.h"
#include "Time.h"

namespace gf {

  class GF_NETWORK_API SocketSelector {
  public:
    SocketSelector() = default;

    void add_socket(Socket& socket);
    void remove_socket(Socket& socket);
    void clear();

    SocketSelectorStatus wait(Time duration = seconds(-1));

    bool ready(Socket& socket);

  private:
    std::vector<pollfd> m_fds;
    bool m_sorted = true;

    decltype(m_fds)::iterator find_socket(Socket& socket);
  };

} // namespace gf

#endif // GF_SOCKET_SELECTOR_H
