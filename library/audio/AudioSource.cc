// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/AudioSource.h>
// clang-format on

#include <gf2/AudioManager.h>
#include <gf2/Log.h>

#include "bits/AudioBackend.h"

namespace gf {

  AudioSource::AudioSource(const std::filesystem::path& filename, uint32_t flags, const AudioSourceContext& context)
  : m_source(std::make_unique<ma_sound>())
  , m_flags(flags)
  {
    using namespace std::literals;

    if (auto result = ma_sound_init_from_file(context.audio_manager.get().engine(), filename.string().c_str(), flags, nullptr, nullptr, m_source.get()); result != MA_SUCCESS) {
      Log::error("Unable to initialize audio source: {}", ma_result_description(result));
      throw std::runtime_error("Unable to initialize audio source: "s + ma_result_description(result));
    }
  }

  AudioSource::AudioSource(AudioSource&&) noexcept = default;

  AudioSource::~AudioSource()
  {
    ma_sound_uninit(m_source.get());
  }

  AudioSource& AudioSource::operator=(AudioSource&&) noexcept = default;

  void AudioSource::start()
  {
    ma_sound_start(m_source.get());
  }

  void AudioSource::stop()
  {
    ma_sound_stop(m_source.get());
  }

}
