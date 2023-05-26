// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_MUSIC_H
#define GF_MUSIC_H

#include "AudioApi.h"
#include "AudioSource.h"

namespace gf {

  class GF_AUDIO_API Music : public AudioSource {
  public:
    using Context = AudioSourceContext;

    Music(const std::filesystem::path& filename, const Context& context);
  };

}

#endif // GF_MUSIC_H
