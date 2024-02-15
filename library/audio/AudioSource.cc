// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/audio/AudioSource.h>

#include <cassert>

#include <gf2/core/Log.h>

#include <gf2/audio/AudioManager.h>

#include "bits/AudioBackend.h"

namespace gf {

  AudioSource::AudioSource(const std::filesystem::path& filename, uint32_t flags, AudioManager* manager)
  : m_source(std::make_unique<ma_sound>())
  , m_flags(flags | MA_SOUND_FLAG_NO_SPATIALIZATION)
  {
    using namespace std::literals;

    assert(manager != nullptr);

    if (auto result = ma_sound_init_from_file(manager->engine(), filename.string().c_str(), m_flags, nullptr, nullptr, m_source.get()); result != MA_SUCCESS) {
      Log::fatal("Unable to initialize audio source: {}", ma_result_description(result));
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

  void AudioSource::set_volume(float volume)
  {
    ma_sound_set_volume(m_source.get(), volume);
  }

  float AudioSource::volume() const
  {
    return ma_sound_get_volume(m_source.get());
  }

  void AudioSource::set_pan(float pan)
  {
    ma_sound_set_pan(m_source.get(), pan);
  }

  float AudioSource::pan() const
  {
    return ma_sound_get_pan(m_source.get());
  }

  void AudioSource::set_pitch(float pitch)
  {
    ma_sound_set_pitch(m_source.get(), pitch);
  }

  float AudioSource::pitch() const
  {
    return ma_sound_get_pitch(m_source.get());
  }

  void AudioSource::set_spatialization_enabled(bool enabled)
  {
    ma_sound_set_spatialization_enabled(m_source.get(), enabled ? MA_TRUE : MA_FALSE);
  }

  bool AudioSource::spatialization_enabled() const
  {
    return ma_sound_is_spatialization_enabled(m_source.get()) == MA_TRUE;
  }

  void AudioSource::set_listener(AudioListener listener)
  {
    ma_sound_set_pinned_listener_index(m_source.get(), listener.m_index);
  }

  AudioListener AudioSource::listener() const
  {
    return AudioListener(ma_sound_get_engine(m_source.get()), ma_sound_get_listener_index(m_source.get()));
  }

  Vec3F AudioSource::direction_to_listener() const
  {
    auto direction = ma_sound_get_direction_to_listener(m_source.get());
    return { direction.x, direction.y, direction.z };
  }

  void AudioSource::set_position(Vec3F position)
  {
    ma_sound_set_position(m_source.get(), position.x, position.y, position.z);
  }

  Vec3F AudioSource::position() const
  {
    auto position = ma_sound_get_position(m_source.get());
    return { position.x, position.y, position.z };
  }

  void AudioSource::set_direction(Vec3F direction)
  {
    ma_sound_set_direction(m_source.get(), direction.x, direction.y, direction.z);
  }

  Vec3F AudioSource::direction() const
  {
    auto direction = ma_sound_get_direction(m_source.get());
    return { direction.x, direction.y, direction.z };
  }

  void AudioSource::set_velocity(Vec3F velocity)
  {
    ma_sound_set_velocity(m_source.get(), velocity.x, velocity.y, velocity.z);
  }

  Vec3F AudioSource::velocity() const
  {
    auto velocity = ma_sound_get_velocity(m_source.get());
    return { velocity.x, velocity.y, velocity.z };
  }

  void AudioSource::set_positioning(AudioPositioning positioning)
  {
    ma_sound_set_positioning(m_source.get(), static_cast<ma_positioning>(positioning));
  }

  AudioPositioning AudioSource::positioning() const
  {
    return static_cast<AudioPositioning>(ma_sound_get_positioning(m_source.get()));
  }

  void AudioSource::set_rolloff(float rolloff)
  {
    ma_sound_set_rolloff(m_source.get(), rolloff);
  }

  float AudioSource::rolloff() const
  {
    return ma_sound_get_rolloff(m_source.get());
  }

  void AudioSource::set_min_gain(float gain)
  {
    ma_sound_set_min_gain(m_source.get(), gain);
  }

  float AudioSource::min_gain() const
  {
    return ma_sound_get_min_gain(m_source.get());
  }

  void AudioSource::set_max_gain(float gain)
  {
    ma_sound_set_max_gain(m_source.get(), gain);
  }

  float AudioSource::max_gain() const
  {
    return ma_sound_get_max_gain(m_source.get());
  }

  void AudioSource::set_min_distance(float distance)
  {
    ma_sound_set_min_distance(m_source.get(), distance);
  }

  float AudioSource::min_distance() const
  {
    return ma_sound_get_min_distance(m_source.get());
  }

  void AudioSource::set_max_distance(float distance)
  {
    ma_sound_set_max_distance(m_source.get(), distance);
  }

  float AudioSource::max_distance() const
  {
    return ma_sound_get_max_distance(m_source.get());
  }

  void AudioSource::set_cone(AudioCone cone)
  {
    ma_sound_set_cone(m_source.get(), cone.inner_angle, cone.outer_angle, cone.outer_gain);
  }

  AudioCone AudioSource::cone() const
  {
    AudioCone cone;
    ma_sound_get_cone(m_source.get(), &cone.inner_angle, &cone.outer_angle, &cone.outer_gain);
    return cone;
  }

  void AudioSource::set_doppler_factor(float factor)
  {
    ma_sound_set_doppler_factor(m_source.get(), factor);
  }

  float AudioSource::doppler_factor() const
  {
    return ma_sound_get_doppler_factor(m_source.get());
  }

  void AudioSource::set_directional_attenuation_factor(float factor)
  {
    ma_sound_set_directional_attenuation_factor(m_source.get(), factor);
  }

  float AudioSource::directional_attenuation_factor() const
  {
    return ma_sound_get_directional_attenuation_factor(m_source.get());
  }

  void AudioSource::set_fade(float volume_from, float volume_to, Time length)
  {
    ma_sound_set_fade_in_milliseconds(m_source.get(), volume_from, volume_to, length.as_milliseconds());
  }

  void AudioSource::set_start_time(Time time)
  {
    ma_sound_set_start_time_in_milliseconds(m_source.get(), time.as_milliseconds());
  }

  void AudioSource::set_stop_time(Time time)
  {
    ma_sound_set_stop_time_in_milliseconds(m_source.get(), time.as_milliseconds());
  }

  bool AudioSource::playing() const
  {
    return ma_sound_is_playing(m_source.get()) == MA_TRUE;
  }

  void AudioSource::set_looping(bool looping)
  {
    ma_sound_set_looping(m_source.get(), looping ? MA_TRUE : MA_FALSE);
  }

  bool AudioSource::looping() const
  {
    return ma_sound_is_looping(m_source.get()) == MA_TRUE;
  }

  bool AudioSource::finished() const
  {
    return ma_sound_at_end(m_source.get());
  }

  Time AudioSource::cursor()
  {
    float cursor = 0;
    ma_sound_get_cursor_in_seconds(m_source.get(), &cursor);
    return seconds(cursor);
  }

  Time AudioSource::length()
  {
    float length = 0;
    ma_sound_get_length_in_seconds(m_source.get(), &length);
    return seconds(length);
  }

}
