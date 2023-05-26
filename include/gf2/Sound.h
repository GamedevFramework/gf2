// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SOUND_H
#define GF_SOUND_H

#include "AudioApi.h"
#include "AudioSource.h"

namespace gf {

  class GF_AUDIO_API Sound : public AudioSource {
  public:
    using Context = AudioSourceContext;

    Sound(const std::filesystem::path& filename, const Context& context);
  };

}

#endif // GF_SOUND_H
