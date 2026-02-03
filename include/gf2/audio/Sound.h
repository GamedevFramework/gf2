// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SOUND_H
#define GF_SOUND_H

#include "AudioApi.h"
#include "AudioSource.h"

namespace gf {
  class AudioManager;

  class GF_AUDIO_API Sound : public AudioSource {
  public:
    using Context = AudioSourceContext;

    Sound(const std::filesystem::path& filename, const AudioSourceContext& context);
  };

}

#endif // GF_SOUND_H
