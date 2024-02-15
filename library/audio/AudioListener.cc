// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/audio/AudioListener.h>

#include "bits/AudioBackend.h"

namespace gf {

  void AudioListener::set_position(Vec3F position)
  {
    ma_engine_listener_set_position(m_engine, m_index, position.x, position.y, position.z);
  }

  Vec3F AudioListener::position() const
  {
    auto position = ma_engine_listener_get_position(m_engine, m_index);
    return { position.x, position.y, position.z };
  }

  void AudioListener::set_direction(Vec3F direction)
  {
    ma_engine_listener_set_direction(m_engine, m_index, direction.x, direction.y, direction.z);
  }

  Vec3F AudioListener::direction() const
  {
    auto direction = ma_engine_listener_get_direction(m_engine, m_index);
    return { direction.x, direction.y, direction.z };
  }

  void AudioListener::set_velocity(Vec3F velocity)
  {
    ma_engine_listener_set_velocity(m_engine, m_index, velocity.x, velocity.y, velocity.z);
  }

  Vec3F AudioListener::velocity() const
  {
    auto velocity = ma_engine_listener_get_velocity(m_engine, m_index);
    return { velocity.x, velocity.y, velocity.z };
  }

  void AudioListener::set_cone(AudioCone cone)
  {
    ma_engine_listener_set_cone(m_engine, m_index, cone.inner_angle, cone.outer_angle, cone.outer_gain);
  }

  AudioCone AudioListener::cone() const
  {
    AudioCone cone;
    ma_engine_listener_get_cone(m_engine, m_index, &cone.inner_angle, &cone.outer_angle, &cone.outer_gain);
    return cone;
  }

  void AudioListener::set_world_up(Vec3F direction)
  {
    ma_engine_listener_set_world_up(m_engine, m_index, direction.x, direction.y, direction.z);
  }

  Vec3F AudioListener::world_up() const
  {
    auto direction = ma_engine_listener_get_world_up(m_engine, m_index);
    return { direction.x, direction.y, direction.z };
  }

  void AudioListener::set_enabled(bool enabled)
  {
    ma_engine_listener_set_enabled(m_engine, m_index, enabled ? MA_TRUE : MA_FALSE);
  }

  bool AudioListener::enabled() const
  {
    return ma_engine_listener_is_enabled(m_engine, m_index) == MA_TRUE;
  }

}
