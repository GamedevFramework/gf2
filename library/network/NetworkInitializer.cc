// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/NetworkInitializer.h>
// clang-format on

#ifdef _WIN32
#include <atomic>
#include <stdexcept>

#include <winsock2.h>

#include <gf2/Log.h>
#endif

namespace gf {

#ifdef _WIN32
  namespace {
    std::atomic_int g_winsock = 0; // NOLINT
  }

  NetworkInitializer::NetworkInitializer()
  {
    if (g_winsock.fetch_add(1) == 0) { // we are the first
      WSADATA wsaData;
      auto res = ::WSAStartup(MAKEWORD(2, 2), &wsaData);

      if (res != 0) {
        Log::error("Unable to initialize the Winsock2 library.");
        throw std::runtime_error("Unable to initialize the Winsock2 library.");
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
    g_loaded.fetch_add(1);
  }

  NetworkInitializer& NetworkInitializer::operator=(NetworkInitializer&& /* unused */) noexcept
  {
    g_loaded.fetch_add(1);
    return *this;
  }
#endif

} // namespace gf
