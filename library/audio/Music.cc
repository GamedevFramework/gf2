// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Music.h>
// clang-format on

#include "bits/AudioBackend.h"

namespace gf {

  Music::Music(const std::filesystem::path& filename, Ref<AudioManager> manager)
  : AudioSource(filename, MA_SOUND_FLAG_STREAM, manager)
  {
  }

}
