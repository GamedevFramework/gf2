// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/audio/AudioManager.h>

#include <cassert>
#include <cstring>

#include <gf2/core/Log.h>

#include "bits/AudioBackend.h"

namespace gf {

  AudioManager::AudioManager()
  : m_engine(std::make_unique<ma_engine>())
  {
    using namespace std::literals;
    if (auto result = ma_engine_init(nullptr, m_engine.get()); result != MA_SUCCESS) {
      Log::fatal("Unable to initialize audio engine: {}", ma_result_description(result));
    }
  }

  AudioManager::AudioManager(AudioManager&& other) noexcept = default;

  AudioManager::~AudioManager()
  {
    ma_engine_uninit(m_engine.get());
  }

  AudioManager& AudioManager::operator=(AudioManager&& other) noexcept = default;

  void AudioManager::set_time(Time global_time)
  {
    if (global_time < Time()) {
      Log::warning("You should set a positive global time in the audio manager.");
      global_time = Time();
    }

    ma_engine_set_time_in_milliseconds(engine(), static_cast<uint64_t>(global_time.as_milliseconds()));
  }

  Time AudioManager::time() const
  {
    uint64_t global_time = ma_engine_get_time_in_milliseconds(engine());
    return gf::milliseconds(static_cast<int32_t>(global_time));
  }

  uint32_t AudioManager::channels() const
  {
    return ma_engine_get_channels(engine());
  }

  uint32_t AudioManager::sample_rate() const
  {
    return ma_engine_get_sample_rate(engine());
  }

  void AudioManager::set_volume(float volume)
  {
    ma_engine_set_volume(engine(), volume);
  }

  AudioListener AudioManager::default_listener()
  {
    return AudioListener(engine(), 0);
  }

  AudioListener AudioManager::closest_listener(Vec3F position)
  {
    return AudioListener(engine(), ma_engine_find_closest_listener(engine(), position.x, position.y, position.z));
  }

  ma_engine* AudioManager::engine()
  {
    return m_engine.get();
  }

  const ma_engine* AudioManager::engine() const
  {
    return m_engine.get();
  }

}
