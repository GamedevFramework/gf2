// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_AUDIO_SOURCE_H
#define GF_AUDIO_SOURCE_H

#include <cstdint>

#include <filesystem>
#include <memory>

#include "AudioApi.h"
#include "Ref.h"

using ma_sound = struct ma_sound;

namespace gf {
  class AudioManager;

  struct GF_AUDIO_API AudioSourceContext {
    Ref<AudioManager> audio_manager;
  };

  class GF_AUDIO_API AudioSource {
  public:
    AudioSource(const AudioSource&) = delete;
    AudioSource(AudioSource&&) noexcept = default;
    ~AudioSource();

    AudioSource& operator=(const AudioSource&) = delete;
    AudioSource& operator=(AudioSource&&) noexcept = default;

    void start();
    void stop();

  protected:
    AudioSource(const std::filesystem::path& filename, uint32_t flags, const AudioSourceContext& context);

  private:
    std::unique_ptr<ma_sound> m_source;
    uint32_t m_flags = 0;
  };

}

#endif // GF_AUDIO_SOURCE_H
