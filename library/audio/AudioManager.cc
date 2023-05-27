// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/AudioManager.h>
// clang-format on

#include <cassert>
#include <cstring>

#include <array>

#include <SDL2/SDL_audio.h>
#include <gf2/Log.h>

// clang-format: off
#define GF_AUDIO_BACKEND_IMPLEMENTATION
#include "bits/AudioBackend.h"
// clang-format on

namespace gf {

  AudioManager::AudioManager()
  : m_engine(std::make_unique<ma_engine>())
  {
    using namespace std::literals;
    if (auto result = ma_engine_init(nullptr, m_engine.get()); result != MA_SUCCESS) {
      Log::error("Unable to initialize audio engine: {}", ma_result_description(result));
      throw std::runtime_error("Unable to initialize audio engine: "s + ma_result_description(result));
    }
  }

  AudioManager::AudioManager(AudioManager&& other) noexcept = default;

  AudioManager::~AudioManager()
  {
    ma_engine_uninit(m_engine.get());
  }

  AudioManager& AudioManager::operator=(AudioManager&& other) noexcept = default;

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
