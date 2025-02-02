// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_EVENTS_H
#define GF_EVENTS_H

#include <optional>

#include <gf2/core/Event.h>

#include "GraphicsApi.h"

namespace gf {

  class GF_GRAPHICS_API Events {
  public:
    constexpr Events() = delete;

    static std::optional<Event> poll();
    static Event wait();

  };

} // namespace gf

#endif // GF_EVENTS_H
