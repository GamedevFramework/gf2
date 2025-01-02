// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Clock.h>

namespace gf {

  Clock::Clock()
  : m_start(details::ClockType::now())
  {
  }

  Time Clock::elapsed_time() const
  {
    auto now = details::ClockType::now();
    auto duration = now - m_start;
    return Time(duration);
  }

  Time Clock::restart()
  {
    auto now = details::ClockType::now();
    auto duration = now - m_start;
    m_start = now;
    return Time(duration);
  }

} // namespace gf
