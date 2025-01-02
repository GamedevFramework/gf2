// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GRAPHICS_INITIALIZER_H
#define GF_GRAPHICS_INITIALIZER_H

#include "GraphicsApi.h"

namespace gf {

  class GF_GRAPHICS_API GraphicsInitializer {
  public:
    GraphicsInitializer();
    ~GraphicsInitializer();

    GraphicsInitializer(const GraphicsInitializer& /* unused */) = delete;
    GraphicsInitializer(GraphicsInitializer&& /* unused */) noexcept = default;

    GraphicsInitializer& operator=(const GraphicsInitializer& /* unused */) = delete;
    GraphicsInitializer& operator=(GraphicsInitializer&& /* unused */) noexcept = default;
  };

} // namespace gf

#endif // GF_GRAPHICS_INITIALIZER_H
