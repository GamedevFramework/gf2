// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_TWEEN_H
#define GF_TWEEN_H

#include <functional>

#include "Easing.h"
#include "Math.h"
#include "Time.h"

namespace gf {

  template<typename T>
  class Tween {
  public:
    using Setter = std::function<void(const T&)>;

    Tween(T origin, T target, Setter setter, Time duration, EasingF easing = ease_linear)
    : m_origin(origin)
    , m_target(target)
    , m_setter(setter)
    , m_duration(duration)
    , m_easing(easing)
    {
    }

    Tween(T origin, T target, T* value, Time duration, EasingF easing = ease_linear)
    : m_origin(origin)
    , m_target(target)
    , m_setter([value](const T& new_value) { if (value != nullptr) { *value = new_value; } })
    , m_duration(duration)
    , m_easing(easing)
    {
    }

    void set_origin(T origin)
    {
      m_origin = origin;
    }

    T origin() const
    {
      return m_origin;
    }

    void set_target(T target)
    {
      m_target = target;
    }

    T target() const
    {
      return m_target;
    }

    void set_duration(Time duration)
    {
      m_duration = duration;
    }

    Time duration() const
    {
      return m_duration;
    }

    T value() const
    {
      return lerp(m_origin, m_target, m_easing(m_elapsed.as_seconds() / m_duration.as_seconds()));
    }

    bool finished() const
    {
      return m_elapsed >= m_duration;
    }

    void update(Time time)
    {
      m_elapsed += time;
      m_elapsed = m_elapsed < m_duration ? m_elapsed : m_duration;
      m_setter(value());
    }

    void restart()
    {
      m_elapsed = Time();
    }

  private:
    T m_origin;
    T m_target;
    Setter m_setter;
    Time m_elapsed;
    Time m_duration;
    EasingF m_easing = ease_linear;
  };

}

#endif // GF_TWEEN_H
