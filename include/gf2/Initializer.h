// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_INITIALIZER_H
#define GF_INITIALIZER_H

#include "GraphicsApi.h"

namespace gf {

  class GF_GRAPHICS_API Initializer {
  public:
    Initializer();
    ~Initializer();

    Initializer(const Initializer& /* unused */) = delete;
    Initializer(Initializer&& /* unused */) noexcept;

    Initializer& operator=(const Initializer& /* unused */) = delete;
    Initializer& operator=(Initializer&& /* unused */) noexcept;
  };

} // namespace gf

#endif // GF_INITIALIZER_H
