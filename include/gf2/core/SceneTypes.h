// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SCENE_TYPES_H
#define GF_SCENE_TYPES_H

#include <cstdint>

namespace gf {

  enum class SceneRank : uint8_t {
    None,
    Active,
    Top,
  };

  enum class SceneStatus : uint8_t {
    Resumed,
    Paused,
  };

  enum class SceneVisibility : uint8_t {
    Shown,
    Hidden,
  };

}

#endif // GF_SCENE_TYPES_H
