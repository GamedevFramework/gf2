// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/network/NetworkInitializer.h>

#ifdef _WIN32
#  include <atomic>

#  include <winsock2.h>

#  include <gf2/core/Log.h>
#endif

namespace gf {

#ifdef _WIN32
  namespace {
    std::atomic_int g_winsock = 0; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
  }

  NetworkInitializer::NetworkInitializer()
  {
    if (g_winsock.fetch_add(1) == 0) { // we are the first
      WSADATA wsaData;
      auto res = ::WSAStartup(MAKEWORD(2, 2), &wsaData);

      if (res != 0) {
        Log::fatal("Unable to initialize the Winsock2 library.");
      }
    }
  }

  NetworkInitializer::~NetworkInitializer()
  {
    if (g_winsock.fetch_sub(1) == 1) { // we are the last
      ::WSACleanup();
    }
  }

  NetworkInitializer::NetworkInitializer(NetworkInitializer&& /* unused */) noexcept
  {
    g_winsock.fetch_add(1);
  }

  NetworkInitializer& NetworkInitializer::operator=(NetworkInitializer&& /* unused */) noexcept
  {
    g_winsock.fetch_add(1);
    return *this;
  }
#endif

} // namespace gf
