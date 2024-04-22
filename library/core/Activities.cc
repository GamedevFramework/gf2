// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/Activities.h>

#include <cassert>
#include <cmath>

namespace gf {

  /*
   * ValueActivity
   */

  ValueActivity::ValueActivity(float origin, float target, float* value, Time duration, Easing easing)
  : m_tween(origin, target, value, duration, easing)
  {
  }

  ActivityStatus ValueActivity::run(Time time)
  {
    if (m_tween.finished()) {
      return ActivityStatus::Finished;
    }

    m_tween.update(time);
    return m_tween.finished() ? ActivityStatus::Finished : ActivityStatus::Running;
  }

  void ValueActivity::restart()
  {
    m_tween.restart();
  }

  /*
   * RotateToActivity
   */

  RotationActivity::RotationActivity(float origin, float target, float* angle, Time duration, Easing easing)
  : m_tween(origin, target, angle, duration, easing)
  {
    normalize();
  }

  ActivityStatus RotationActivity::run(Time time)
  {
    if (m_tween.finished()) {
      return ActivityStatus::Finished;
    }

    m_tween.update(time);
    return m_tween.finished() ? ActivityStatus::Finished : ActivityStatus::Running;
  }

  void RotationActivity::restart()
  {
    m_tween.restart();
  }

  void RotationActivity::normalize()
  {
    const float origin = m_tween.origin();
    float target = m_tween.target();

    target = origin + std::remainder(target - origin, 2 * Pi);
    assert(origin - gf::Pi <= target && target <= origin + Pi);

    m_tween.set_target(target);
  }

  /*
   * MoveToActivity
   */

  MotionActivity::MotionActivity(Vec2F origin, Vec2F target, Vec2F* position, Time duration, Easing easing)
  : m_tween(origin, target, position, duration, easing)
  {
  }

  ActivityStatus MotionActivity::run(Time time)
  {
    if (m_tween.finished()) {
      return ActivityStatus::Finished;
    }

    m_tween.update(time);
    return m_tween.finished() ? ActivityStatus::Finished : ActivityStatus::Running;
  }

  void MotionActivity::restart()
  {
    m_tween.restart();
  }

  /*
   * ColorActivity
   */

  ColorActivity::ColorActivity(Color origin, Color target, Color* color, Time duration, Easing easing)
  : m_tween(origin, target, color, duration, easing)
  {
  }

  ActivityStatus ColorActivity::run(Time time)
  {
    if (m_tween.finished()) {
      return ActivityStatus::Finished;
    }

    m_tween.update(time);
    return m_tween.finished() ? ActivityStatus::Finished : ActivityStatus::Running;
  }

  void ColorActivity::restart()
  {
    m_tween.restart();
  }

  /*
   * CallbackActivity
   */

  ActivityStatus CallbackActivity::run([[maybe_unused]] Time time)
  {
    if (!m_called) {
      m_callback();
      m_called = true;
    }

    return ActivityStatus::Finished;
  }

  void CallbackActivity::restart()
  {
    m_called = false;
  }

  /*
   * DelayActivity
   */

  DelayActivity::DelayActivity(Time duration)
  : m_duration(duration)
  {
  }

  ActivityStatus DelayActivity::run(Time time)
  {
    m_elapsed += time;

    if (m_elapsed > m_duration) {
      return ActivityStatus::Finished;
    }

    return ActivityStatus::Running;
  }

  void DelayActivity::restart()
  {
    m_elapsed = Time();
  }

  /*
   * SequenceActivity
   */

  void SequenceActivity::add_activity(Activity* activity)
  {
    if (activity != nullptr) {
      m_activities.push_back(activity);
    }
  }

  void SequenceActivity::clear()
  {
    m_activities.clear();
  }

  ActivityStatus SequenceActivity::run(Time time)
  {
    if (m_current == m_activities.size()) {
      return ActivityStatus::Finished;
    }

    Activity* activity = m_activities[m_current];
    const ActivityStatus status = activity->run(time);

    if (status == ActivityStatus::Finished) {
      ++m_current;
    }

    return m_current == m_activities.size() ? ActivityStatus::Finished : ActivityStatus::Running;
  }

  void SequenceActivity::restart()
  {
    m_current = 0;

    for (Activity* activity : m_activities) {
      activity->restart();
    }
  }

  /*
   * RepeatActivity
   */

  RepeatActivity::RepeatActivity(Activity* activity, int repeat)
  : m_activity(activity)
  , m_repeat(repeat)
  {
  }

  ActivityStatus RepeatActivity::run(Time time)
  {
    if (m_activity == nullptr || (m_repeat > 0 && m_repeat == m_count)) {
      return ActivityStatus::Finished;
    }

    const ActivityStatus status = m_activity->run(time);

    if (status == ActivityStatus::Finished) {
      m_activity->restart();
      ++m_count;
    }

    return (m_repeat > 0 && m_repeat == m_count) ? ActivityStatus::Finished : ActivityStatus::Running;
  }

  void RepeatActivity::restart()
  {
    m_count = 0;
    m_activity->restart();
  }

  /*
   * ParallelActivity
   */

  ParallelActivity::ParallelActivity(ActivityFinish finish)
  : m_finish(finish)
  {
  }

  void ParallelActivity::add_activity(Activity* activity)
  {
    if (activity != nullptr) {
      m_activities.push_back(activity);
    }
  }

  void ParallelActivity::clear()
  {
    m_activities.clear();
  }

  ActivityStatus ParallelActivity::run(Time time)
  {
    if (m_status == ActivityStatus::Finished) {
      return ActivityStatus::Finished;
    }

    std::size_t finished = 0;

    for (Activity* activity : m_activities) {
      const ActivityStatus status = activity->run(time);

      if (status == ActivityStatus::Finished) {
        ++finished;
      }
    }

    switch (m_finish) {
      case ActivityFinish::Any:
        if (finished > 0) {
          m_status = ActivityStatus::Finished;
        }
        break;

      case ActivityFinish::All:
        if (finished == m_activities.size()) {
          m_status = ActivityStatus::Finished;
        }
        break;
    }

    return m_status;
  }

  void ParallelActivity::restart()
  {
    m_status = ActivityStatus::Running;

    for (Activity* activity : m_activities) {
      activity->restart();
    }
  }

}
