// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef AUDIO_SPATIALIZATION_H
#define AUDIO_SPATIALIZATION_H

#include <cstdint>

namespace gf {

  struct AudioCone {
    float inner_angle;
    float outer_angle;
    float outer_gain;
  };

  enum class AudioPositioning : uint8_t {
    Absolute,
    Relative,
  };

}

#endif // AUDIO_SPATIALIZATION_H
