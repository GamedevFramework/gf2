// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_AUDIO_SOURCE_H
#define GF_AUDIO_SOURCE_H

#include <cstdint>

#include <filesystem>
#include <memory>

#include <gf2/core/Time.h>
#include <gf2/core/Vec3.h>

#include "AudioApi.h"
#include "AudioListener.h"
#include "AudioSpatialization.h"

using ma_sound = struct ma_sound;

namespace gf {
  class AudioManager;

  struct AudioSourceContext {
    bool loop = false;
    float volume = 1.0f;
    AudioManager* audio_manager = nullptr;
  };

  class GF_AUDIO_API AudioSource {
  public:
    AudioSource(const AudioSource&) = delete;
    AudioSource(AudioSource&&) noexcept;
    ~AudioSource();

    AudioSource& operator=(const AudioSource&) = delete;
    AudioSource& operator=(AudioSource&&) noexcept;

    void start();
    void stop();

    void set_volume(float volume);
    float volume() const;

    void set_pan(float pan);
    float pan() const;

    void set_pitch(float pitch);
    float pitch() const;

    void set_spatialization_enabled(bool enabled);
    bool spatialization_enabled() const;

    void set_listener(AudioListener listener);
    AudioListener listener() const;

    Vec3F direction_to_listener() const;

    void set_position(Vec3F position);
    Vec3F position() const;

    void set_direction(Vec3F direction);
    Vec3F direction() const;

    void set_velocity(Vec3F velocity);
    Vec3F velocity() const;

    void set_positioning(AudioPositioning positioning);
    AudioPositioning positioning() const;

    void set_rolloff(float rolloff);
    float rolloff() const;

    void set_min_gain(float gain);
    float min_gain() const;

    void set_max_gain(float gain);
    float max_gain() const;

    void set_min_distance(float distance);
    float min_distance() const;

    void set_max_distance(float distance);
    float max_distance() const;

    void set_cone(AudioCone cone);
    AudioCone cone() const;

    void set_doppler_factor(float factor);
    float doppler_factor() const;

    void set_directional_attenuation_factor(float factor);
    float directional_attenuation_factor() const;

    void set_fade(float volume_from, float volume_to, Time length);

    void set_start_time(Time time);
    void set_stop_time(Time time);

    bool playing() const;

    void set_looping(bool looping);
    bool looping() const;

    bool finished() const;

    Time cursor();
    Time length();

  protected:
    AudioSource(const std::filesystem::path& filename, uint32_t flags, AudioManager* manager);

  private:
    std::unique_ptr<ma_sound> m_source;
    uint32_t m_flags = 0;
  };

}

#endif // GF_AUDIO_SOURCE_H
