// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_ACTIVITY_H
#define GF_ACTIVITY_H

#include <cstdint>

#include "CoreApi.h"
#include "Time.h"

namespace gf {

  enum class ActivityStatus : uint8_t {
    Running,
    Finished,
  };

  class GF_CORE_API Activity {
  public:
    Activity() = default;
    Activity(const Activity&) = default;
    Activity(Activity&&) noexcept = default;
    virtual ~Activity();

    Activity& operator=(const Activity&) = default;
    Activity& operator=(Activity&&) noexcept = default;

    virtual ActivityStatus run(Time time) = 0;
    virtual void restart();
  };

}

#endif // GF_ACTIVITY_H
