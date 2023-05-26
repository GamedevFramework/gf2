// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_AUDIO_INITIALIZER_H
#define GF_AUDIO_INITIALIZER_H

#include "AudioApi.h"

namespace gf {

  class GF_AUDIO_API AudioInitializer {
  public:
    AudioInitializer();
    ~AudioInitializer();

    AudioInitializer(const AudioInitializer& /* unused */) = delete;
    AudioInitializer(AudioInitializer&& /* unused */) noexcept = default;

    AudioInitializer& operator=(const AudioInitializer& /* unused */) = delete;
    AudioInitializer& operator=(AudioInitializer&& /* unused */) noexcept = default;
  };

} // namespace gf

#endif // GF_GRAPHICS_INITIALIZER_H
