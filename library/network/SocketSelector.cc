// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/SocketSelector.h>
// clang-format on

#include <algorithm>

#include <gf2/Log.h>

#include "bits/SocketPortability.h"

namespace gf {

  namespace {

    struct PollFdEqualTo {
      explicit PollFdEqualTo(details::SocketHandle reference)
      : m_reference(reference)
      {
      }

      bool operator()(const pollfd& other) const
      {
        return m_reference == other.fd;
      }

    private:
      details::SocketHandle m_reference;
    };

    struct PollFdLess {
      bool operator()(const pollfd& lhs, const pollfd& rhs) const
      {
        return lhs.fd < rhs.fd;
      }

      bool operator()(details::SocketHandle lhs, const pollfd& rhs) const
      {
        return lhs < rhs.fd;
      }

      bool operator()(const pollfd& lhs, details::SocketHandle rhs) const
      {
        return lhs.fd < rhs;
      }
    };

  } // namespace

  void SocketSelector::add_socket(Socket& socket)
  {
    m_fds.push_back(pollfd{ socket.m_handle, POLLIN, 0 });
    m_sorted = false;
  }

  void SocketSelector::remove_socket(Socket& socket)
  {
    m_fds.erase(std::remove_if(m_fds.begin(), m_fds.end(), PollFdEqualTo(socket.m_handle)), m_fds.end());
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
      std::sort(m_fds.begin(), m_fds.end(), PollFdLess());
      m_sorted = true;
    }

    return details::native_poll(m_fds, duration);
  }

  bool SocketSelector::ready(Socket& socket)
  {
    if (auto iterator = find_socket(socket); iterator != m_fds.end()) {
      return iterator->revents != 0;
    }

    Log::error("Error! Searching for a socket that is not in the selector.");
    return false;
  }

  auto SocketSelector::find_socket(Socket& socket) -> decltype(m_fds)::iterator
  {
    if (m_sorted) {
      auto iterator = std::lower_bound(m_fds.begin(), m_fds.end(), socket.m_handle, PollFdLess());

      if (iterator != m_fds.end() && iterator->fd == socket.m_handle) {
        return iterator;
      }

      return m_fds.end();
    }

    return std::find_if(m_fds.begin(), m_fds.end(), PollFdEqualTo(socket.m_handle));
  }

} // namespace gf
