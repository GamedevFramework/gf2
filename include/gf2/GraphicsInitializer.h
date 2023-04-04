// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_GRAPHICS_INITIALIZER_H
#define GF_GRAPHICS_INITIALIZER_H

#include "GraphicsApi.h"

namespace gf {

  class GF_GRAPHICS_API GraphicsInitializer {
  public:
    GraphicsInitializer();
    ~GraphicsInitializer();

    GraphicsInitializer(const GraphicsInitializer&) = delete;
    GraphicsInitializer(GraphicsInitializer&&) noexcept;

    GraphicsInitializer& operator=(const GraphicsInitializer&) = delete;
    GraphicsInitializer& operator=(GraphicsInitializer&&) noexcept;

    static bool initialized();
  };

} // namespace gf

#endif // GF_GRAPHICS_INITIALIZER_H
