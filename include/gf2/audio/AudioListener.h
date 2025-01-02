// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_AUDIO_LISTENER_H
#define GF_AUDIO_LISTENER_H

#include <cstdint>

#include <gf2/core/Vec3.h>

#include "AudioApi.h"
#include "AudioSpatialization.h"

using ma_engine = struct ma_engine;

namespace gf {

  class GF_AUDIO_API AudioListener {
  public:
    void set_position(Vec3F position);
    Vec3F position() const;

    void set_direction(Vec3F direction);
    Vec3F direction() const;

    void set_velocity(Vec3F velocity);
    Vec3F velocity() const;

    void set_cone(AudioCone cone);
    AudioCone cone() const;

    void set_world_up(Vec3F direction);
    Vec3F world_up() const;

    void set_enabled(bool enabled);
    bool enabled() const;

  private:
    AudioListener(ma_engine* engine, uint32_t index)
    : m_engine(engine)
    , m_index(index)
    {
    }

    friend class AudioManager;
    friend class AudioSource;
    ma_engine* m_engine = nullptr;
    uint32_t m_index = 0;
  };

}

#endif // GF_AUDIO_LISTENER_H
