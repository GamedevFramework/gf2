// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SOUND_H
#define GF_SOUND_H

#include "AudioApi.h"
#include "AudioSource.h"

namespace gf {
  class AudioManager;

  class GF_AUDIO_API Sound : public AudioSource {
  public:
    using Context = Ref<AudioManager>;

    Sound(const std::filesystem::path& filename, Ref<AudioManager> manager);
  };

}

#endif // GF_SOUND_H
