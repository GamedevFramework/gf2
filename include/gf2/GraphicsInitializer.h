// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_GRAPHICS_INITIALIZER_H
#define GF_GRAPHICS_INITIALIZER_H

#include "GraphicsApi.h"
#include "Initializer.h"

namespace gf {

  class GF_GRAPHICS_API GraphicsInitializer : public Initializer {
  public:
    GraphicsInitializer();
  };

} // namespace gf

#endif // GF_GRAPHICS_INITIALIZER_H
