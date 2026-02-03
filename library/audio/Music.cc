// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/audio/Music.h>

#include "bits/AudioBackend.h"

namespace gf {

  Music::Music(const std::filesystem::path& filename, const AudioSourceContext& context)
  : AudioSource(filename, MA_SOUND_FLAG_STREAM, context.audio_manager)
  {
    set_looping(context.loop);
    set_volume(context.volume);
  }

}
