// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_AUDIO_MANAGER_H
#define GF_AUDIO_MANAGER_H

#include <memory>
#include <string>

#include "AudioApi.h"

using ma_engine = struct ma_engine;

namespace gf {

  class GF_AUDIO_API AudioManager {
  public:
    AudioManager();
    AudioManager(const AudioManager&) = delete;
    AudioManager(AudioManager&& other) noexcept = default;
    ~AudioManager();

    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager& operator=(AudioManager&& other) noexcept = default;

    std::string device_name() const;

  private:
    friend class AudioSource;
    struct AudioSystem;

    ma_engine* engine();

    std::unique_ptr<AudioSystem> m_system;
  };

}

#endif // GF_AUDIO_MANAGER_H
