// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SHADER_STAGE_H
#define GF_SHADER_STAGE_H

#include <type_traits>

namespace gf {

  // NOLINTNEXTLINE(performance-enum-size)
  enum class ShaderStage {
    Vertex,
    Fragment,
  };

}

#endif // GF_SHADER_STAGE_H
