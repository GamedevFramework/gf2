// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Clock.h>
// clang-format on

namespace gf {

  Clock::Clock()
  : m_start(std::chrono::steady_clock::now())
  {
  }

  Time Clock::elapsed_time() const
  {
    auto now = std::chrono::steady_clock::now();
    auto duration = now - m_start;
    return Time(duration);
  }

  Time Clock::restart()
  {
    auto now = std::chrono::steady_clock::now();
    auto duration = now - m_start;
    m_start = now;
    return Time(duration);
  }

} // namespace gf
