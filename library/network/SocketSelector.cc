// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/network/SocketSelector.h>

#include <algorithm>

#include <gf2/core/Log.h>

#include "bits/SocketPortability.h"

namespace gf {

  void SocketSelector::add_socket(const Socket& socket)
  {
    m_fds.push_back(pollfd{ socket.m_handle, POLLIN, 0 });
    m_sorted = false;
  }

  void SocketSelector::remove_socket(const Socket& socket)
  {
    auto removed_fd = std::ranges::remove(m_fds, socket.m_handle, &pollfd::fd);
    m_fds.erase(removed_fd.begin(), removed_fd.end());
    // the vector is still sorted (or unsorted)
  }

  void SocketSelector::clear()
  {
    m_fds.clear();
    m_sorted = true;
  }

  SocketSelectorStatus SocketSelector::wait(Time duration)
  {
    if (m_fds.empty()) {
      Log::error("Error! The selector does not contain any socket.");
      return SocketSelectorStatus::Error;
    }

    if (!m_sorted) {
      std::ranges::sort(m_fds, {}, &pollfd::fd);
      m_sorted = true;
    }

    return details::native_poll(m_fds, duration);
  }

  bool SocketSelector::ready(const Socket& socket)
  {
    if (auto iterator = find_socket(socket); iterator != m_fds.end()) {
      return iterator->revents != 0;
    }

    Log::error("Error! Searching for a socket that is not in the selector.");
    return false;
  }

  auto SocketSelector::find_socket(const Socket& socket) -> decltype(m_fds)::iterator
  {
    if (m_sorted) {
      auto iterator = std::ranges::lower_bound(m_fds, socket.m_handle, {}, &pollfd::fd);

      if (iterator != m_fds.end() && iterator->fd == socket.m_handle) {
        return iterator;
      }

      return m_fds.end();
    }

    return std::ranges::find(m_fds, socket.m_handle, &pollfd::fd);
  }

} // namespace gf
