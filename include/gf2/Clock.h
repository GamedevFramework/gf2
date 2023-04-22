// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CLOCK_H
#define GF_CLOCK_H

#include <chrono>

#include "CoreApi.h"
#include "Time.h"

namespace gf {

  class GF_CORE_API Clock {
  public:
    Clock();

    Time elapsed_time() const;
    Time restart();

  private:
    std::chrono::steady_clock::time_point m_start;
  };

} // namespace gf

#endif // GF_CLOCK_H
