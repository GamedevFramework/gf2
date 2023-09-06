// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_AUDIO_MANAGER_H
#define GF_AUDIO_MANAGER_H

#include <cstdint>

#include <memory>
#include <string>

#include <gf2/core/Time.h>
#include <gf2/core/Vec3.h>

#include "AudioApi.h"
#include "AudioListener.h"

using ma_engine = struct ma_engine;

namespace gf {

  class GF_AUDIO_API AudioManager {
  public:
    AudioManager();
    AudioManager(const AudioManager&) = delete;
    AudioManager(AudioManager&& other) noexcept;
    ~AudioManager();

    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager& operator=(AudioManager&& other) noexcept;

    void set_time(Time global_time);
    Time time() const;

    uint32_t channels() const;
    uint32_t sample_rate() const;

    void set_volume(float volume);

    AudioListener default_listener();
    AudioListener closest_listener(Vec3F position);

  private:
    friend class AudioSource;

    ma_engine* engine();
    const ma_engine* engine() const;

    std::unique_ptr<ma_engine> m_engine;
  };

}

#endif // GF_AUDIO_MANAGER_H
