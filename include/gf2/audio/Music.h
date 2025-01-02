// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_MUSIC_H
#define GF_MUSIC_H

#include "AudioApi.h"
#include "AudioSource.h"

namespace gf {

  class GF_AUDIO_API Music : public AudioSource {
  public:
    using Context = AudioManager*;

    Music(const std::filesystem::path& filename, AudioManager* manager);
  };

}

#endif // GF_MUSIC_H
