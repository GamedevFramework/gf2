// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Sound.h>
// clang-format on

#include "bits/AudioBackend.h"

namespace gf {

  Sound::Sound(const std::filesystem::path& filename, AudioManager* manager)
  : AudioSource(filename, MA_SOUND_FLAG_DECODE, manager)
  {
  }

}
