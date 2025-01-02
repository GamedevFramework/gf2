// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/audio/Sound.h>

#include "bits/AudioBackend.h"

namespace gf {

  Sound::Sound(const std::filesystem::path& filename, AudioManager* manager)
  : AudioSource(filename, MA_SOUND_FLAG_DECODE, manager)
  {
  }

}
